module objects
  implicit none
  
  integer, parameter :: dp = selected_real_kind(15,307)
  integer, parameter :: lng = selected_int_kind(8)

  type part
    real(dp) :: pos(2);
    real(dp) :: mass;
  end type

  type node
    ! properties of the node
    real(dp) :: halfDim ! half side of the node
    real(dp) :: origin(2) ! origin of the node
    type(part) :: particle ! particle location
    type(part) :: CM ! node center of mass 

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
    type(part), intent(in)             :: p

    type(node), pointer :: child_1, child_2, child_3, child_4, Next_N1, &
      Next_N2
    real(dp)   :: neworigin(2), Madd, Mnew, Mold
    type(part) :: p_old 
    integer    :: i

    if (N%leaf_node) then
      ! if we are in a leaf node there are no child nodes
      if (.not. N%contains_particle) then
        ! if no particles, just put particle in this node
        N%contains_particle = .true.
        N%CM = p 
      else
        ! otherwise we need to split up the node into quadrants, 
        ! and try to put new point there, and move old point to
        ! appropriate quadrant as well

        ! save position data
        p_old = N%particle
        N%leaf_node = .false.
        N%contains_particle = .false.
        ! add mass to node
        Mold = N%CM%mass
        Madd = p%mass
        Mnew = Mold + Madd
        ! update center of mass position
        N%CM%pos = (Mold*N%CM%pos + Madd*p%pos)/Mnew
        ! update mass of node
        N%CM%mass = Mnew

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
      ! still need to update CM though
      Mold = N%CM%mass
      Madd = p%mass
      Mnew = Mold + Madd
      ! update center of mass position
      N%CM%pos = (Mold*N%CM%pos + Madd*p%pos)/Mnew
      ! update mass of node
      N%CM%mass = Mnew

      call get_quadrant(p, N, Next_N1)
      call insert_particle(p, Next_N1) 
    endif
  end subroutine

  subroutine get_quadrant(p, N, Next_N)
    ! returns pointer to quadrant where p is located
    type(node), pointer    :: Next_N
    type(node), pointer    :: N
    type(part), intent(in) :: p

    if(p%pos(1) > N%origin(1) .and. p%pos(2) < N%origin(2)) then
      Next_N => N%quadrant2
    elseif(p%pos(1) < N%origin(1) .and. p%pos(2) < N%origin(2)) then
      Next_N => N%quadrant3
    elseif(p%pos(1) < N%origin(1) .and. p%pos(2) > N%origin(2)) then
      Next_N => N%quadrant4
    else 
      Next_N => N%quadrant1
    endif
  end subroutine

  recursive subroutine getforce(N,p,F)
    ! lists locations of particles contained in the tree
    real(dp),   intent(inout) :: F(:)
    type(part), intent(in)    :: p
    type(node), pointer       :: N

    real(dp) :: dr(2), d, m1, m2

    if (N%leaf_node) then
      if (N%contains_particle) then
        m1 = N%particle%mass
        m2 = p%mass
        dr = p%pos - N%particle%pos

        d = sqrt(sum(dr**2));
        if (d<0.0001) then ! if particles are too close ignore
          F = F
        else
          F = F - dr*m1*m2/d**3
        endif
      endif
    else 
      dr = p%pos - N%CM%pos;  
      d = sqrt(sum(dr**2));

      if (N%halfDim/d < 0.5_dp) then
        m1 = N%CM%mass
        m2 = p%mass
        F = F - dr*m1*m2/d**3
      else
        call getforce(N%quadrant1,p,F)
        call getforce(N%quadrant2,p,F)
        call getforce(N%quadrant3,p,F)
        call getforce(N%quadrant4,p,F)
      endif
    endif
  end subroutine

  subroutine mkbox(p, N)
    ! initiates box with correct dimensions, origin for root 
    real(dp), intent(in) :: p(:,:)
    type(node), pointer  :: N

    N%halfDim = 1.1_dp*max(maxval(abs(p(:,1))), maxval(abs(p(:,2))))
    N%origin  = [0._dp, 0._dp]
  end subroutine
end module

program tree
  use objects
  implicit none
  
  ! first node: the root of the tree
  type(node), pointer   :: root 
  ! list of points
  type(part) :: p1, p2, p3, p4
  real(dp)   :: F(3) = 0._dp
  allocate(root)

  p1%pos = [0._dp, 0._dp]
  p1%mass = 1._dp
  p2%pos = [1._dp, 0._dp]
  p2%mass = 1._dp
  p3%pos = [-1._dp, 0._dp]
  p3%mass = 1._dp
  p4%pos = [1._dp, 0.2_dp]
  p4%mass = 0._dp

  root%halfDim = 2._dp
  root%origin = [0._dp, 0._dp]
  
  ! create root box that can hold all coords
  !call mkbox(p, root)
  
  ! add a bunch of particles to tree
  call insert_particle(p1, root)
  call insert_particle(p2, root)
  call insert_particle(p3, root)
  call insert_particle(p4, root)

  call getforce(root,p1,F)
  print *, F
end program
