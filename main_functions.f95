module main_functions
  use constants
  implicit none
  private
  public :: fold, rescale, meas_T
  
contains
  pure subroutine rescale(p, T, T_tgt)
    ! rescales momentum to keep temperature fixed, berendsen thermostat
    real(dp), intent(inout) :: p(:,:)
    real(dp), intent(in) :: T_tgt, T
    real(dp) :: lambda

    lambda = sqrt(1._dp + 0.0025_dp*(T_tgt/T-1._dp))
    ! lambda = sqrt(T_tgt/T)
    p = p*lambda
  end subroutine  

  pure subroutine fold(r, fold_count, L)
    ! enforce periodic BC on positions, track of number of shifts
    real(dp), intent(inout) :: r(:,:)
    real(dp), intent(in) :: L
    integer, intent(inout) :: fold_count(:,:)

    fold_count = fold_count + floor(r/L)
    r = r - floor(r/L)*L
  end subroutine

  pure function meas_T(p)
    real(dp), intent(in) :: p(:,:)
    real(dp) :: meas_T
    
    meas_T = sum(p**2)/(3._dp*(N-1))
  end function
end module
