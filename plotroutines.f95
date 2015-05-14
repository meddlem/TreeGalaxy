module plotroutines
  use constants
  use treestructs
  use plplot
  implicit none
  private
  public :: particle_plot, particle_plot_init

contains
  subroutine particle_plot_init(xmin,xmax)
    real(dp), intent(in) :: xmin, xmax
    
    call Colors()
    call plsdev("xcairo")
    call plinit()
    call pladv(0)
    
    ! define viewport, world coords for the edges
    call plvpor(0._dp, 1._dp, 0._dp, 0.9_dp)
    call plwind(-1._dp,1._dp,-1._dp,1.5_dp)
    call plw3d(1._dp, 1._dp, 1.2_dp, xmin, xmax, xmin, xmax, xmin, xmax, &
      20._dp, 45._dp)
    call plspause(.false.)
  end subroutine
 
  subroutine particle_plot(r)
    ! plots all particle position
    type(part), intent(in) :: r(:) 
    
    real(dp) :: r_p(N,3)
    integer :: i
    ! convert part type to regular double array
    do i = 1,N
      r_p(i,:) = r(i)%pos
    enddo
   
    call plclear()
    call plcol0(1) !axis color
    call plbox3("", "", 0._dp, 0, "", "", 0._dp, 0, "", "", &
      0._dp, 0) !plots the axes etc
    call plcol0(3) !point color
    call plpoin3(r_p(:,3), r_p(:,2), r_p(:,1), 1) !this plots the points
    call plflush()
  end subroutine
  
  subroutine Colors()
    ! redefining plplot colors
    call plscol0(0,255,255,255)
    call plscol0(1,255,0,0)
    call plscol0(2,0,255,0)
    call plscol0(3,0,0,255)
    call plscol0(4,255,0,255)
    call plscol0(5,0,255,255)
    call plscol0(6,255,255,0)
    call plscol0(7,0,0,0)
    call plscol0(8,255,70,0)
    call plscol0(9,128,128,128)
  end subroutine
end module 
