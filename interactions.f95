module interactions
  use constants
  use omp_lib
  use tree
  implicit none
  private 
  public :: force

contains
  subroutine force(F, r)
    ! computes net force on particles
    real(dp), intent(out)   :: F(:,:)
    type(part), intent(in)  :: r(:)

    type(node), pointer :: root 
    integer :: i

    ! create the root of the tree
    allocate(root)
    call mkbox(r, root)

    ! set force to 0
    F = 0._dp

    ! add particles to tree
    do i = 1,N 
      call insert_particle(r(i), root)
    enddo
    
    ! calculate forces
    do i = 1,N 
      call getforce(root,r(i),F(i,:))
    enddo
  end subroutine
end module 
