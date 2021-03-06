module tree
  use constants
  use treestructs
  implicit none
  private
  public :: insert_particle, calc_force, mkbox, destroytree

contains
  
  subroutine mkbox(p, M)
    ! initiates box with correct dimensions, origin for root 
    type(part), intent(in) :: p(:)
    type(node), pointer    :: M

    M%halfDim = 1.1_dp*maxval([maxval(abs(p%pos(1))), maxval(abs(p%pos(2))), &
      maxval(abs(p%pos(3)))])
    M%origin  = [0._dp, 0._dp, 0._dp]
  end subroutine

  recursive subroutine insert_particle(p, M)
    type(node), pointer, intent(inout) :: M
    type(part), intent(in)             :: p

    type(node), pointer :: child_1, child_2, child_3, child_4, child_5, &
      child_6, child_7, child_8, Next_M1, Next_M2
    real(dp)   :: Madd, Mnew, Mold
    type(part) :: p_old 

    if (M%leaf_node) then
      ! if we are in a leaf node there are no child nodes
      if (.not. M%contains_particle) then
        ! if no particles, just put particle in this node
        M%contains_particle = .true.
        M%CM = p 
        M%particle = p 
      else
        ! otherwise we need to split up the node into octants, 
        ! and try to put new point there, and move old point to
        ! appropriate octant as well

        ! save position data
        p_old = M%particle
        M%leaf_node = .false.
        M%contains_particle = .false.
        ! add mass to node
        Mold = M%CM%mass
        Madd = p%mass
        Mnew = Mold + Madd
        ! update center of mass position
        M%CM%pos = (Mold*M%CM%pos + Madd*p%pos)/Mnew
        ! update mass of node
        M%CM%mass = Mnew

        ! allocate children 
        allocate(child_1, child_2, child_3, child_4, child_5, child_6, &
          child_7, child_8)

        ! assign pointers to children
        M%octant1 => child_1
        M%octant2 => child_2
        M%octant3 => child_3
        M%octant4 => child_4
        M%octant5 => child_5
        M%octant6 => child_6
        M%octant7 => child_7
        M%octant8 => child_8

        ! give children origins, sizes
        call init_children(M, child_1, child_2, child_3, child_4, child_5, &
          child_6, child_7, child_8)

        ! now we find the octant the points are in (p_old and p) and
        ! insert them in the appropriate child nodes that we just created
        call get_octant(p_old, M, Next_M1)
        call insert_particle(p_old, Next_M1)

        call get_octant(p, M, Next_M2)
        call insert_particle(p, Next_M2)
      endif
    else
      ! we are at an interior node, insert p into one of its octants
      ! still need to update CM though
      Mold = M%CM%mass
      Madd = p%mass
      Mnew = Mold + Madd
      ! update center of mass position
      M%CM%pos = (Mold*M%CM%pos + Madd*p%pos)/Mnew
      ! update mass of node
      M%CM%mass = Mnew

      call get_octant(p, M, Next_M1)
      call insert_particle(p, Next_M1) 
    endif
  end subroutine

  subroutine init_children(M, child_1, child_2, child_3, child_4, child_5, &
      child_6, child_7, child_8)
    type(node), pointer :: M, child_1, child_2, child_3, child_4, child_5, &
      child_6, child_7, child_8

    real(dp) :: O(3), d

    O = M%origin
    d = M%halfDim

    ! determine new origins and box sizes for children
    child_1%origin(1) = O(1) + d/sqrt(3._dp) 
    child_1%origin(2) = O(2) + d/sqrt(3._dp) 
    child_1%origin(3) = O(3) + d/sqrt(3._dp) 
    child_1%halfDim = d/2._dp

    child_2%origin(1) = O(1) + d/sqrt(3._dp) 
    child_2%origin(2) = O(2) - d/sqrt(3._dp) 
    child_2%origin(3) = O(3) + d/sqrt(3._dp) 
    child_2%halfDim = d/2._dp
  
    child_3%origin(1) = O(1) - d/sqrt(3._dp) 
    child_3%origin(2) = O(2) - d/sqrt(3._dp) 
    child_3%origin(3) = O(3) + d/sqrt(3._dp) 
    child_3%halfDim = d/2._dp

    child_4%origin(1) = O(1) - d/sqrt(3._dp) 
    child_4%origin(2) = O(2) + d/sqrt(3._dp) 
    child_4%origin(3) = O(3) + d/sqrt(3._dp) 
    child_4%halfDim = d/2._dp
    
    child_5%origin(1) = O(1) + d/sqrt(3._dp) 
    child_5%origin(2) = O(2) + d/sqrt(3._dp) 
    child_5%origin(3) = O(3) - d/sqrt(3._dp) 
    child_5%halfDim = d/2._dp
    
    child_6%origin(1) = O(1) + d/sqrt(3._dp) 
    child_6%origin(2) = O(2) - d/sqrt(3._dp) 
    child_6%origin(3) = O(3) - d/sqrt(3._dp) 
    child_6%halfDim = d/2._dp

    child_7%origin(1) = O(1) - d/sqrt(3._dp) 
    child_7%origin(2) = O(2) - d/sqrt(3._dp) 
    child_7%origin(3) = O(3) - d/sqrt(3._dp) 
    child_7%halfDim = d/2._dp

    child_8%origin(1) = O(1) - d/sqrt(3._dp)
    child_8%origin(2) = O(2) + d/sqrt(3._dp)
    child_8%origin(3) = O(3) - d/sqrt(3._dp)
    child_8%halfDim = d/2._dp
  end subroutine

  subroutine get_octant(p, M, Next_M)
    ! returns pointer to octant where p is located
    type(node), pointer    :: Next_M
    type(node), pointer    :: M
    type(part), intent(in) :: p

    real(dp) :: O(3)
    
    O = M%origin

    if(p%pos(1) > O(1) .and. p%pos(2) < O(2) .and. p%pos(3) > O(3)) then
      Next_M => M%octant2
    elseif(p%pos(1) < O(1) .and. p%pos(2) < O(2) .and. p%pos(3) > O(3)) then
      Next_M => M%octant3
    elseif(p%pos(1) < O(1) .and. p%pos(2) > O(2) .and. p%pos(3) > O(3)) then
      Next_M => M%octant4
    elseif(p%pos(1) > O(1) .and. p%pos(2) > O(2) .and. p%pos(3) < O(3)) then
      Next_M => M%octant5
    elseif(p%pos(1) > O(1) .and. p%pos(2) < O(2) .and. p%pos(3) < O(3)) then
      Next_M => M%octant6
    elseif(p%pos(1) < O(1) .and. p%pos(2) < O(2) .and. p%pos(3) < O(3)) then
      Next_M => M%octant7
    elseif(p%pos(1) < O(1) .and. p%pos(2) > O(2) .and. p%pos(3) < O(3)) then
      Next_M => M%octant8
    else 
      Next_M => M%octant1
    endif
  end subroutine

  recursive subroutine calc_force(M,p,F)
    ! lists locations of particles contained in the tree
    real(dp),   intent(inout) :: F(:)
    type(part), intent(in)    :: p
    type(node), pointer       :: M

    real(dp) :: dr(3), d, m1, m2

    if (M%leaf_node) then
      if (M%contains_particle .and. (p%pos(1) /= M%particle%pos(1))) then
        m1 = M%particle%mass
        m2 = p%mass
        ! calc distance between particles
        dr = p%pos - M%particle%pos
        d = sqrt(sum(dr**2));

        if (d>0.0001_dp) then ! if particles are too close ignore
          F = F - Gr*dr*m1*m2/(d**2 + eps**2)**1.5_dp
        endif
      endif
    else 
      dr = p%pos - M%CM%pos;  
      d = sqrt(sum(dr**2));

      if (2._dp*M%halfDim/d < theta) then
        ! we treat the node as a single particle 
        m1 = M%CM%mass
        m2 = p%mass
        F = F - Gr*dr*m1*m2/(d**2 + eps**2)**1.5_dp
      else
        call calc_force(M%octant1, p, F)
        call calc_force(M%octant2, p, F)
        call calc_force(M%octant3, p, F)
        call calc_force(M%octant4, p, F)
        call calc_force(M%octant5, p, F)
        call calc_force(M%octant6, p, F)
        call calc_force(M%octant7, p, F)
        call calc_force(M%octant8, p, F)
      endif
    endif
  end subroutine

  recursive subroutine destroytree(M)
    ! deallocates tree structure recursively
    type(node), pointer :: M

    if (associated(M%octant1)) then
      call destroytree(M%octant1)
    endif

    if (associated(M%octant2)) then
      call destroytree(M%octant2)
    endif

    if (associated(M%octant3)) then
      call destroytree(M%octant3)
    endif

    if (associated(M%octant4)) then
      call destroytree(M%octant4)
    endif

    if (associated(M%octant5)) then
      call destroytree(M%octant5)
    endif

    if (associated(M%octant6)) then
      call destroytree(M%octant6)
    endif

    if (associated(M%octant7)) then
      call destroytree(M%octant7)
    endif

    if (associated(M%octant8)) then
      call destroytree(M%octant8)
    endif

    deallocate(M)
  end subroutine
end module
