program main
  ! modules to use 
  use constants 
  use io
  use initialize 
  use main_functions 
  use plotroutines 
  use interactions 
  use plplot, only : plend  
  implicit none

  real(dp), allocatable :: r(:,:), r_0(:,:), p(:,:), p_0(:,:), F(:,:) 
  real(dp) :: L, rho, T_init
  integer :: i
  ! allocate large arrays
  allocate(r(N,3), r_0(N,3), p(N,3), p_0(N,3), F(N,3))
  
  ! get required userinput 
  call user_in(rho,T_init)  

  ! initialize a few needed variables 
  L = (N/rho)**(1._dp/3._dp)

  ! initialize the model
  call init_r(r,L)
  call init_p(p,T_init)
  call force(F,r)
  if(prtplt) call particle_plot_init(-10._dp*L,10._dp*L)
  
  ! central part of simulation
  do i = 1,steps
    ! plot particle positions
    if (prtplt .or. mod(i,10) == 0) call particle_plot(r) 
     
    ! time integration using the "velocity Verlet" algorithm: 
    !r = r + p*dt + 0.5_dp*F*(dt**2) ! update positions
    !p = p + 0.5_dp*F*dt ! update momentum (1/2)
    !call force(F,r) ! update force
    !p = p + 0.5_dp*F*dt ! update momentum (2/2)
    ! leapfrog 
    r = r + 0.5_dp*p*dt
    call force(F,r) ! update force
    p = p + F*dt
  enddo
  
  call plend()
end program main 
