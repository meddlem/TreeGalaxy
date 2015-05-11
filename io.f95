module io
  use constants
  implicit none
  private
  public :: user_in
contains

  subroutine user_in(rho,T_init)
    real(dp), intent(out) :: rho, T_init
  
    write(*,'(/,A,/)') '************ Input *************' 
    write(*,'(A)',advance='no') "Number density = " 
    read(*,*) rho
    write(*,'(A)',advance='no') "Init temperature = " 
    read(*,*) T_init
    write(*,'(A,I5,A)') "Starting simulation: ", steps, " iterations"
  end subroutine
end module
