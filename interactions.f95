module interactions
  use constants
  use omp_lib
  implicit none
  private 
  public :: force

contains
  subroutine force(F, r)
    ! computes net force on particles
    real(dp), intent(out) :: F(:,:)
    real(dp), intent(in)  :: r(:,:)
    real(dp), allocatable :: FMAT(:,:,:)
    real(dp) :: d, dr(3), m, eps
    integer :: i, j

    ! initialize, allocate large array
    allocate(FMAT(N,N,3))
    FMAT = 0._dp
    m = 1._dp
    eps = 0.05_dp ! softening parameter

    do i = 1,N
      do j = (i+1),N
        dr = r(i,:) - r(j,:) 
        d = sqrt(sum(dr**2)) 
        FMAT(i,j,:) = -m*dr/(d**2 + eps**2)**1.5_dp
        FMAT(j,i,:) = -FMAT(i,j,:) !use newton 3
      enddo
    enddo
    
    ! calculate total force vectors
    F = sum(FMAT,2) 
    deallocate(FMAT)
  end subroutine
end module 
