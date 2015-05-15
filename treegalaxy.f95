program treegalaxy
  use iso_c_binding
  use constants 
  use treestructs
  use initialize 
  use interactions 
  implicit none

  ! define interfaces to the C++ code (used for plotting with openGL)
  interface 
    subroutine render(p, N) bind(c,name='Render')
      import :: c_int
      import :: c_ptr 
      type(c_ptr), value :: p
      integer(c_int), value :: N
    end subroutine

    subroutine pre_render() bind(c,name='Pre_Render')
    end subroutine

    subroutine post_render() bind(c,name='Post_Render')
    end subroutine
  end interface

  ! main program

  real(dp), allocatable   :: v(:,:), F(:,:) 
  type(part), allocatable :: r(:)

  allocate(r(N), v(N,3), F(N,3))
  
  call init(r, v)
  call run_sim(r, v, F)

contains 

  subroutine run_sim(r, v, F)
    type(part), intent(inout) :: r(:)
    real(dp), intent(inout)   :: v(:,:), F(:,:)

    integer :: i, j
    real(c_float), allocatable, target :: rc(:,:)
    integer(c_int) :: NC
    type(c_ptr) :: cptr

    allocate(rc(3,N))
    cptr = c_loc(rc(1,1))
    NC = N

    call force(F,r)
    call pre_render()
    
    do i = 1,steps
      ! plot particle positions
      do j = 1,N
        rc(:,j) = real(r(j)%pos)
      enddo
      call render(cptr, N)

      ! time integration using the "velocity Verlet" algorithm: 
      do j = 1,N
        r(j)%pos = r(j)%pos + v(j,:)*dt + 0.5_dp*F(j,:)*(dt**2)/r(j)%mass
      enddo

      do j = 1,N
        v(j,:) = v(j,:) + 0.5_dp*F(j,:)*dt/r(j)%mass ! update momentum (1/2)
      enddo

      call force(F, r) 
      
      do j = 1,N
        v(j,:) = v(j,:) + 0.5_dp*F(j,:)*dt/r(j)%mass ! update momentum (1/2)
      enddo
    enddo
    
  call post_render()

  end subroutine
end program 
