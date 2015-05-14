module constants
  implicit none
  ! module contains all constants used in the program
  ! units: eps=1, sigma=1, m=1
  
  ! NOTE: IF YOU MAKE ANY CHANGES HERE RECOMPILE ALL MODULES: "make -B" 
  integer, parameter :: dp = selected_real_kind(15,307)
  integer, parameter :: lng = selected_int_kind(8)
  real(dp), parameter :: pi = 4._dp*atan(1._dp) 

  real(dp), parameter :: dt = 0.05_dp 
  real(dp), parameter :: eps = 0.025_dp ! softening parameter 
  real(dp), parameter :: theta = 0.5_dp ! cutoff 
  
  integer, parameter :: steps = 30000
  integer, parameter :: N = 20480
  
  logical, parameter :: prtplt = .true.
end module
