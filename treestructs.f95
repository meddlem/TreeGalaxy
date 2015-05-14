module treestructs
  use constants
  implicit none

  type part
    real(dp) :: pos(3);
    real(dp) :: mass = 0._dp;
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
end module
