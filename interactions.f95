module interactions
  use constants
  use treestructs
  use tree
  implicit none
  private 
  public :: force

contains
  subroutine force(F, r)
    real(dp), intent(inout) :: F(:,:)
    type(part), intent(in)  :: r(:)

    type(node), pointer :: root 
    integer :: i

    ! create the root of the tree
    allocate(root)
    call mkbox(r, root)

    ! add particles to tree
    do i = 1,N 
      call insert_particle(r(i), root)
    enddo
    
    ! calculate forces
    F = 0._dp
    do i = 1,N 
      call calc_force(root, r(i), F(i,:))
    enddo
    
    if (maxval(F) > 100000._dp) then
      print *, "ebin"
    endif

    ! deallocate tree recursively
    call destroytree(root)
  end subroutine
end module 
