module objects
  implicit none
  
  integer, parameter     :: dp = selected_real_kind(15,307)
  integer, parameter     :: lng = selected_int_kind(8)

  type node
    real(dp) :: halfDim 
    real(dp) :: origin(2) ! origin of the node
    real(dp) :: loc(2) ! particle location

    logical  :: contains_particle = .false.
    logical  :: leaf_node = .true.

    ! pointers to 4 subcells
    type(node), pointer :: quadrant1 => null()
    type(node), pointer :: quadrant2 => null()
    type(node), pointer :: quadrant3 => null()
    type(node), pointer :: quadrant4 => null()
  end type

contains
  recursive subroutine insert_particle(p, N)
    type(node), pointer, intent(inout) :: N
    real(dp), intent(in)               :: p(2)

    type(node), pointer :: child_1, child_2, child_3, child_4, Next_N
    real(dp)   :: neworigin(2), p_old(2)
    integer    :: i

    if (N%leaf_node) then
      ! if we are in a leaf node, ie no child nodes
      if (.not. N%contains_particle) then
        ! put particle in this node
        N%contains_particle = .true.
        N%loc = p
      else
        ! save position data
        p_old = N%loc
        N%leaf_node = .false.
        ! allocate children
        allocate(child_1)
        allocate(child_2)
        allocate(child_3)
        allocate(child_4)
        N%quadrant1 => child_1
        N%quadrant2 => child_2
        N%quadrant3 => child_3
        N%quadrant4 => child_4

        do i = 1,4
          neworigin(1) = N%origin(1) + &
            N%halfDim/sqrt(2._dp) * merge(1, -1, i == 1 .or. i == 4)
          neworigin(2) = N%origin(2) + &
            N%halfDim/sqrt(2._dp) * merge(1, -1, i < 3)
          
          if (i == 1) then
            child_1%origin = neworigin
            child_1%halfDim = N%halfDim/2._dp

          elseif (i == 2) then 
            child_2%origin = neworigin
            child_2%halfDim = N%halfDim/2._dp
          elseif (i == 3) then 
            child_3%origin = neworigin
            child_3%halfDim = N%halfDim/2._dp
          else 
            child_4%origin = neworigin
            child_4%halfDim = N%halfDim/2._dp
          endif
        enddo
        
        ! now we need to quadrant the points are in (p_temp and p) and
        ! insert them in the child nodes that we just created
        call get_quadrant(p_old, N, Next_N)
        call insert_particle(p_old, Next_N)

        call get_quadrant(p, N, Next_N)
        call insert_particle(p, Next_N)
      endif
    else
      ! we are at an interior node,
      ! insert into one of its quadrants
      call get_quadrant(p, N, Next_N)
      call insert_particle(p, Next_N) 
    endif
  end subroutine

  subroutine get_quadrant(p, N, Child_N)
    ! returns pointer to quadrant where p is located
    type(node), pointer  :: Child_N
    type(node), pointer  :: N
    real(dp), intent(in) :: p(2)

    if(p(1) > N%origin(1) .and. p(2) < N%origin(2)) then
      Child_N => N%quadrant2
    elseif(p(1) < N%origin(1) .and. p(2) > N%origin(2)) then
      Child_N => N%quadrant4
    elseif(p(1) < N%origin(1) .and. p(2) < N%origin(2)) then
      Child_N => N%quadrant3
    else 
      Child_N => N%quadrant1
    endif
  end subroutine

  ! lists locations of particles contained in the tree
  recursive subroutine getpoints(N)
    type(node), pointer :: N

    !type(node), pointer :: Child_N

    if (N%leaf_node .and. N%contains_particle) then
      print *, N%loc
    elseif(.not. N%leaf_node) then
      call getpoints(N%quadrant1)
      call getpoints(N%quadrant2)
      call getpoints(N%quadrant3)
      call getpoints(N%quadrant4)
    endif
  end subroutine
end module

program tree
  use objects
  implicit none
  
  ! first node: the root of the tree
  type(node), pointer   :: root 
  real(dp)              :: p1(2), p2(2), p3(2)

  allocate(root)
  root%halfDim = 2._dp
  root%origin  = [0._dp, 0._dp]

  ! add a bunch of particles 
  p1 = [1._dp, 0._dp]
  p2 = [0._dp, 1._dp]
  p3 = [-0.4_dp, -1._dp]

  call insert_particle(p1, root)
  call insert_particle(p2, root)
  call insert_particle(p3, root)
  call getpoints(root)
end program
