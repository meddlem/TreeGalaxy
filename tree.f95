module tree
  use constants
  implicit none

  type part
    real(dp) :: pos(3);
    real(dp) :: mass;
  end type

  type node
    ! properties of the node
    real(dp) :: halfDim ! half side of the node
    real(dp) :: origin(3) ! origin of the node
    type(part) :: particle ! particle location
    type(part) :: CM ! node center of mass 

    logical  :: contains_particle = .false. 
    logical  :: leaf_node = .true.

    ! pointers to 4 subnodes
    type(node), pointer :: octant1 => null()
    type(node), pointer :: octant2 => null()
    type(node), pointer :: octant3 => null()
    type(node), pointer :: octant4 => null()
    type(node), pointer :: octant5 => null()
    type(node), pointer :: octant6 => null()
    type(node), pointer :: octant7 => null()
    type(node), pointer :: octant8 => null()
  end type

contains
  recursive subroutine insert_particle(p, N)
    type(node), pointer, intent(inout) :: N
    type(part), intent(in)             :: p

    type(node), pointer :: child_1, child_2, child_3, child_4, child_5, &
      child_6, child_7, child_8, Next_N1, Next_N2
    real(dp)   :: Madd, Mnew, Mold
    type(part) :: p_old 

    if (N%leaf_node) then
      ! if we are in a leaf node there are no child nodes
      if (.not. N%contains_particle) then
        ! if no particles, just put particle in this node
        N%contains_particle = .true.
        N%CM = p 
      else
        ! otherwise we need to split up the node into octants, 
        ! and try to put new point there, and move old point to
        ! appropriate octant as well

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
        allocate(child_1, child_2, child_3, child_4, child_5, child_6, &
          child_7, child_8)

        ! assign pointers to children
        N%octant1 => child_1
        N%octant2 => child_2
        N%octant3 => child_3
        N%octant4 => child_4
        N%octant5 => child_5
        N%octant6 => child_6
        N%octant7 => child_7
        N%octant8 => child_8

        ! determine new origins and box sizes for children
        child_1%origin(1) = N%origin(1) + N%halfDim/sqrt(3._dp) 
        child_1%origin(2) = N%origin(2) + N%halfDim/sqrt(3._dp) 
        child_1%origin(3) = N%origin(3) + N%halfDim/sqrt(3._dp) 
        child_1%halfDim = N%halfDim/2._dp

        child_2%origin(1) = N%origin(1) + N%halfDim/sqrt(3._dp) 
        child_2%origin(2) = N%origin(2) - N%halfDim/sqrt(3._dp) 
        child_2%origin(3) = N%origin(3) + N%halfDim/sqrt(3._dp) 
        child_2%halfDim = N%halfDim/2._dp
      
        child_3%origin(1) = N%origin(1) - N%halfDim/sqrt(3._dp) 
        child_3%origin(2) = N%origin(2) - N%halfDim/sqrt(3._dp) 
        child_3%origin(3) = N%origin(3) + N%halfDim/sqrt(3._dp) 
        child_3%halfDim = N%halfDim/2._dp

        child_4%origin(1) = N%origin(1) - N%halfDim/sqrt(3._dp) 
        child_4%origin(2) = N%origin(2) + N%halfDim/sqrt(3._dp) 
        child_4%origin(3) = N%origin(3) + N%halfDim/sqrt(3._dp) 
        child_4%halfDim = N%halfDim/2._dp
        
        child_5%origin(1) = N%origin(1) + N%halfDim/sqrt(3._dp) 
        child_5%origin(2) = N%origin(2) + N%halfDim/sqrt(3._dp) 
        child_5%origin(3) = N%origin(3) - N%halfDim/sqrt(3._dp) 
        child_5%halfDim = N%halfDim/2._dp
        
        child_6%origin(1) = N%origin(1) + N%halfDim/sqrt(3._dp) 
        child_6%origin(2) = N%origin(2) - N%halfDim/sqrt(3._dp) 
        child_6%origin(3) = N%origin(3) - N%halfDim/sqrt(3._dp) 
        child_6%halfDim = N%halfDim/2._dp

        child_7%origin(1) = N%origin(1) - N%halfDim/sqrt(3._dp) 
        child_7%origin(2) = N%origin(2) - N%halfDim/sqrt(3._dp) 
        child_7%origin(3) = N%origin(3) - N%halfDim/sqrt(3._dp) 
        child_7%halfDim = N%halfDim/2._dp

        child_8%origin(1) = N%origin(1) - N%halfDim/sqrt(3._dp) 
        child_8%origin(2) = N%origin(2) + N%halfDim/sqrt(3._dp) 
        child_8%origin(3) = N%origin(3) - N%halfDim/sqrt(3._dp) 
        child_8%halfDim = N%halfDim/2._dp

        ! now we find the octant the points are in (p_old and p) and
        ! insert them in the appropriate child nodes that we just created
        call get_octant(p_old, N, Next_N1)
        call insert_particle(p_old, Next_N1)

        call get_octant(p, N, Next_N2)
        call insert_particle(p, Next_N2)
      endif
    else
      ! we are at an interior node, insert p into one of its octants
      ! still need to update CM though
      Mold = N%CM%mass
      Madd = p%mass
      Mnew = Mold + Madd
      ! update center of mass position
      N%CM%pos = (Mold*N%CM%pos + Madd*p%pos)/Mnew
      ! update mass of node
      N%CM%mass = Mnew

      call get_octant(p, N, Next_N1)
      call insert_particle(p, Next_N1) 
    endif
  end subroutine

  subroutine get_octant(p, N, Next_N)
    ! returns pointer to octant where p is located
    type(node), pointer    :: Next_N
    type(node), pointer    :: N
    type(part), intent(in) :: p

    real(dp) :: O(3)
    
    O = N%origin

    if(p%pos(1) > O(1) .and. p%pos(2) < O(2) .and. p%pos(3) > O(3)) then
      Next_N => N%octant2
    elseif(p%pos(1) < O(1) .and. p%pos(2) < O(2) .and. p%pos(3) > O(3)) then
      Next_N => N%octant3
    elseif(p%pos(1) < O(1) .and. p%pos(2) > O(2) .and. p%pos(3) > O(3)) then
      Next_N => N%octant4
    elseif(p%pos(1) > O(1) .and. p%pos(2) > O(2) .and. p%pos(3) < O(3)) then
      Next_N => N%octant5
    elseif(p%pos(1) > O(1) .and. p%pos(2) < O(2) .and. p%pos(3) < O(3)) then
      Next_N => N%octant6
    elseif(p%pos(1) < O(1) .and. p%pos(2) < O(2) .and. p%pos(3) < O(3)) then
      Next_N => N%octant7
    elseif(p%pos(1) < O(1) .and. p%pos(2) > O(2) .and. p%pos(3) < O(3)) then
      Next_N => N%octant8
    else 
      Next_N => N%octant1
    endif
  end subroutine

  recursive subroutine getforce(N,p,F)
    ! lists locations of particles contained in the tree
    real(dp),   intent(inout) :: F(:)
    type(part), intent(in)    :: p
    type(node), pointer       :: N

    real(dp) :: dr(3), d, m1, m2

    if (N%leaf_node) then
      if (N%contains_particle) then
        m1 = N%particle%mass
        m2 = p%mass
        dr = p%pos - N%particle%pos

        d = sqrt(sum(dr**2));
        if (d<0.001) then ! if particles are too close ignore
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
        call getforce(N%octant1,p,F)
        call getforce(N%octant2,p,F)
        call getforce(N%octant3,p,F)
        call getforce(N%octant4,p,F)
        call getforce(N%octant5,p,F)
        call getforce(N%octant6,p,F)
        call getforce(N%octant7,p,F)
        call getforce(N%octant8,p,F)
      endif
    endif
  end subroutine

  subroutine mkbox(p, N)
    ! initiates box with correct dimensions, origin for root 
    type(part), intent(in) :: p(:)
    type(node), pointer    :: N

    N%halfDim = 1.1_dp*maxval([maxval(abs(p%pos(1))), maxval(abs(p%pos(2))), &
      maxval(abs(p%pos(3)))])
    N%origin  = [0._dp, 0._dp, 0._dp]
  end subroutine
end module

!program tree
!  use objects
!  implicit none
  
  ! first node: the root of the tree
!  type(node), pointer :: root 
  ! list of points
!  type(part), allocatable :: p(:)
!  real(dp), allocatable   :: F(:,:)
!  integer :: i, N = 1000
!  allocate(root, p(N), F(3,N))

  ! set force to 0
!  F = 0._dp

!  call mkbox(p, root)
  
  ! add a bunch of particles to tree
!  do i = 1,N 
!    call insert_particle(p(i), root)
!  enddo
  
  ! calculate forces
!  do i = 1,N 
!    call getforce(root,p(i),F(:,i))
!  enddo
!end program
