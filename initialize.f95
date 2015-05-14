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
    integer :: i, j

    ! read data from Dubinski 1995
    open(10, file='dubinski.tab')
      do i = 1,N
        read(10,*) r(i)%mass, r(i)%pos(1), r(i)%pos(2), r(i)%pos(3), v(i,1), v(i,2), v(i,3)
        do j = 1,3
          read(10,*)  
        enddo
      enddo 
    close(10)

    r%mass = 4._dp*r%mass ! rescale
  end subroutine  
end module 
