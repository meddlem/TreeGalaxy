module initialize
  use constants
  use treestructs
  implicit none
  private
  public :: init

contains
  subroutine init(r,v) 
    ! gives initial positions based on FCC lattice
    type(part), intent(inout) :: r(:)
    real(dp), intent(inout)   :: v(:,:)
    integer :: i

    r%mass = 0.01_dp !masses

    open(10, file='stars.dat')
      do i=1,N
        read(10,*) r(i)%pos(3), r(i)%pos(2), r(i)%pos(1), &
          v(i,3), v(i,2), v(i,1)
      enddo 
    close(10)
  end subroutine  
end module 
