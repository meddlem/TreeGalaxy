program main
  ! modules to use 
  use constants 
  use treestructs
  use initialize 
  use plotroutines 
  use interactions 
  use plplot, only : plend  
  implicit none

  real(dp), allocatable   :: v(:,:), F(:,:) 
  type(part), allocatable :: r(:)
  integer  :: i, j
  ! allocate large arrays
  allocate(r(N), v(N,3), F(N,3))
  
  ! initialize the model
  call init(r,v)
  call force(F,r)
  if(prtplt) call particle_plot_init(-20._dp,20.1_dp)
  
  ! central part of simulation
  do i = 1,steps
    ! plot particle positions
    if(prtplt .and. mod(i,2)==0) call particle_plot(r) 

    ! time integration using the "velocity Verlet" algorithm: 
    do j = 1,N
      ! update positions
      r(j)%pos = r(j)%pos + v(j,:)*dt + 0.5_dp*F(j,:)*(dt**2)
    enddo
    v = v + 0.5_dp*F*dt ! update momentum (1/2)
    !call particle_plot(r) 
    call force(F,r) ! update force
    v = v + 0.5_dp*F*dt ! update momentum (2/2)
  enddo
  
  call plend()
end program main 
