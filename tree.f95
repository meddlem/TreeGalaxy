module objects
  implicit none
  
  integer, parameter :: dp = selected_real_kind(15,307)
  integer, parameter :: lng = selected_int_kind(8)

  type node
    ! properties of the node
    real(dp) :: halfDim ! half side of the node
    real(dp) :: origin(2) ! origin of the node
    real(dp) :: loc(2) ! node center of mass 
    !(if 1 particle in node, this is just particle location) 
    real(dp) :: mass   ! node mass (mass of all particles in node)

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
      ! if we are in a leaf node there are no child nodes
      if (.not. N%contains_particle) then
        ! if no particles, just put particle in this node
        N%contains_particle = .true.
        N%loc = p
      else
        ! otherwise we need to split up the node into quadrants, 
        ! and try to put new point there, and move old point to
        ! appropriate quadrant as well

        ! save position data
        p_old = N%loc
        N%leaf_node = .false.

        ! allocate children 
        allocate(child_1, child_2, child_3, child_4)

        ! assign pointers to children
        N%quadrant1 => child_1
        N%quadrant2 => child_2
        N%quadrant3 => child_3
        N%quadrant4 => child_4

        ! determine new origins and box sizes for children
        child_1%origin(1) = N%origin(1) + N%halfDim/sqrt(2._dp) 
        child_1%origin(2) = N%origin(2) + N%halfDim/sqrt(2._dp) 
        child_1%halfDim = N%halfDim/2._dp

        child_2%origin(1) = N%origin(1) + N%halfDim/sqrt(2._dp) 
        child_2%origin(2) = N%origin(2) - N%halfDim/sqrt(2._dp) 
        child_2%halfDim = N%halfDim/2._dp
      
        child_3%origin(1) = N%origin(1) - N%halfDim/sqrt(2._dp) 
        child_3%origin(2) = N%origin(2) - N%halfDim/sqrt(2._dp) 
        child_3%halfDim = N%halfDim/2._dp

        child_4%origin(1) = N%origin(1) - N%halfDim/sqrt(2._dp) 
        child_4%origin(2) = N%origin(2) + N%halfDim/sqrt(2._dp) 
        child_4%halfDim = N%halfDim/2._dp
        
        ! now we find the quadrant the points are in (p_old and p) and
        ! insert them in the appropriate child nodes that we just created
        call get_quadrant(p_old, N, Next_N1)
        call insert_particle(p_old, Next_N1)

        call get_quadrant(p, N, Next_N2)
        call insert_particle(p, Next_N2)
      endif
    else
      ! we are at an interior node, insert p into one of its quadrants
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
    elseif(p(1) < N%origin(1) .and. p(2) < N%origin(2)) then
      Next_N => N%quadrant3
    elseif(p(1) < N%origin(1) .and. p(2) > N%origin(2)) then
      Next_N => N%quadrant4
    else 
      Next_N => N%quadrant1
    endif
  end subroutine

  recursive subroutine getpoints(N)
    ! lists locations of particles contained in the tree
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

  subroutine mkbox(p, N)
    ! initiates box with correct dimensions, origin for root 
    real(dp), intent(in) :: p(:,:)
    type(node), pointer  :: N

    N%halfDim = 1.1_dp*max(maxval(abs(p(:,1))), maxval(abs(p(:,2))))
    N%origin  = [0._dp, 0._dp]
  end subroutine

  subroutine get_rel_distance(p0, N, d)
    real(dp), intent(in)    :: p0(:)
    real(dp), intent(inout) :: d(:, :)
    type(node), pointer     :: N


  end subroutine
end module

program tree
  use objects
  implicit none
  
  ! first node: the root of the tree
  type(node), pointer   :: root 
  ! list of points
  real(dp), allocatable :: p(:,:)

  allocate(root, p(4,2))

  p(1,:) = [1._dp, 0.5_dp]
  p(2,:) = [0.5_dp, 1._dp]
  p(3,:) = [0.4_dp, 1.1_dp]
  p(4,:) = [1._dp, 0.2_dp]
  
  ! create root box that can hold all coords
  call mkbox(p, root)
  
  ! add a bunch of particles to tree
  call insert_particle(p(1,:), root)
  call insert_particle(p(2,:), root)
  call insert_particle(p(3,:), root)
  call insert_particle(p(4,:), root)
  ! and read them again
  call getpoints(root)
  ! get relative distances from point p_0 to all points in tree
end program
