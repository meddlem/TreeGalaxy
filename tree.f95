module objects
  implicit none
  
  integer, parameter     :: dp = selected_real_kind(15,307)
  integer, parameter     :: lng = selected_int_kind(8)

  type node
    real(dp) :: halfDim ! half side of the node
    real(dp) :: origin(2) ! origin of the node
  
    real(dp) :: loc(2) ! particle location
    logical  :: contains_particle = .false.
    logical  :: leaf_node = .true.

    ! pointers to 4 subnodes
    type(node), pointer :: quadrant1 => null()
    type(node), pointer :: quadrant2 => null()
    type(node), pointer :: quadrant3 => null()
    type(node), pointer :: quadrant4 => null()
  end type

contains
  recursive subroutine insert_particle(p, N)
    type(node), pointer, intent(inout) :: N
    real(dp), intent(in)               :: p(2)

    type(node), pointer :: child_1, child_2, child_3, child_4, Next_N1, &
      Next_N2
    real(dp)   :: neworigin(2), p_old(2)
    integer    :: i

    if (N%leaf_node) then
      ! if we are in a leaf node, ie no child nodes
      if (.not. N%contains_particle) then
        ! put particle in this node
        print *, 'test'
        N%contains_particle = .true.
        N%loc = p
      else
        ! save position data
        p_old = N%loc
        N%leaf_node = .false.

        ! allocate children probably doesnt work because of this, 
        ! cant have multiple pointers with the same name?
        allocate(child_1)
        allocate(child_2)
        allocate(child_3)
        allocate(child_4)

        ! create pointers to children
        N%quadrant1 => child_1
        N%quadrant2 => child_2
        N%quadrant3 => child_3
        N%quadrant4 => child_4

        child_1%origin(1) = N%origin(1) + N%halfDim/sqrt(2._dp) 
        child_1%origin(2) = N%origin(2) + N%halfDim/sqrt(2._dp) 

        child_2%origin(1) = N%origin(1) + N%halfDim/sqrt(2._dp) 
        child_2%origin(2) = N%origin(2) - N%halfDim/sqrt(2._dp) 
      
        child_3%origin(1) = N%origin(1) - N%halfDim/sqrt(2._dp) 
        child_3%origin(2) = N%origin(2) - N%halfDim/sqrt(2._dp) 

        child_4%origin(1) = N%origin(1) - N%halfDim/sqrt(2._dp) 
        child_4%origin(2) = N%origin(2) + N%halfDim/sqrt(2._dp) 

        child_1%halfDim = N%halfDim/2._dp
        child_2%halfDim = N%halfDim/2._dp
        child_3%halfDim = N%halfDim/2._dp
        child_4%halfDim = N%halfDim/2._dp
        
        ! now we need to quadrant the points are in (p_old and p) and
        ! insert them in the child nodes that we just created
        call get_quadrant(p_old, N, Next_N1)
        call insert_particle(p_old, Next_N1)

        call get_quadrant(p, N, Next_N2)
        call insert_particle(p, Next_N2)
      endif
    else
      ! we are at an interior node,
      ! insert p into one of its quadrants
      call get_quadrant(p, N, Next_N1)
      call insert_particle(p, Next_N1) 
    endif
  end subroutine

  subroutine get_quadrant(p, N, Next_N)
    ! returns pointer to quadrant where p is located
    type(node), pointer  :: Next_N
    type(node), pointer  :: N
    real(dp), intent(in) :: p(2)

    if(p(1) > N%origin(1) .and. p(2) < N%origin(2)) then
      Next_N => N%quadrant2
    elseif(p(1) < N%origin(1) .and. p(2) > N%origin(2)) then
      Next_N => N%quadrant4
    elseif(p(1) < N%origin(1) .and. p(2) < N%origin(2)) then
      Next_N => N%quadrant3
    else 
      Next_N => N%quadrant1
    endif
  end subroutine

  ! lists locations of particles contained in the tree
  recursive subroutine getpoints(N)
    type(node), pointer :: N

    if (N%leaf_node .and. N%contains_particle) then
      print *, N%loc
    elseif(.not. N%leaf_node) then
      ! call recursively until we reach the leaf
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
  ! bunch of points
  real(dp)              :: p1(2), p2(2), p3(2), p4(2)

  allocate(root)
  ! need routine to determine these from point data
  root%halfDim = 2._dp
  root%origin  = [0._dp, 0._dp]

  ! add a bunch of particles 
  p1 = [1._dp, 0.5_dp]
  p2 = [0.5_dp, 1._dp]
  p3 = [0.4_dp, 1.1_dp]
  p4 = [1._dp, 0.2_dp]

  call insert_particle(p1, root)
  call insert_particle(p2, root)
  call insert_particle(p3, root)
  call insert_particle(p4, root)
  call getpoints(root)
end program
