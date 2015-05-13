module initialize
  use constants
  use treestructs
  implicit none
  private
  public :: init

contains
  subroutine init(r,v) 
    type(part), intent(inout) :: r(:)
    real(dp), intent(inout)   :: v(:,:)
    integer :: i

    r%mass = 0.005_dp !masses, kind of need to guess right now

    ! read data from Sellwood 1993
    open(10, file='stars.dat')
      do i=1,N
        read(10,*) r(i)%pos(3), r(i)%pos(2), r(i)%pos(1), &
          v(i,3), v(i,2), v(i,1)
      enddo 
    close(10)
  end subroutine  
end module 
