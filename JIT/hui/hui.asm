section .text

global hui

hui:
  mov rax, 17000
  CVTSI2SD xmm0, rax
  mov rax, 1000
  ;call hui

  mov rax, -1
  mov rax, -23932423455675
  mov rcx, 23932423455675
  mov rdx, 23932423455675
  mov rbx, 23932423455675
  mov rcx, 500
  mov rdx, 500
  mov rbx, 500
  
  mov rax, rax
  mov rax, rcx
  mov rax, rdx
  mov rax, rbx 

  mov rcx, rax
  mov rcx, rcx
  mov rcx, rdx
  mov rcx, rbx 
  
  mov rdx, rax
  mov rdx, rcx
  mov rdx, rdx
  mov rdx, rbx 
  
  cmp rax, rax
  cmp rax, rcx
  cmp rax, rdx
  cmp rax, rbx
  
  cmp rcx, rax
  cmp rcx, rcx
  cmp rcx, rdx
  cmp rcx, rbx

  cmp rdx, rax
  cmp rdx, rcx
  cmp rdx, rdx
  cmp rdx, rbx

  cmp rdx, rax
  cmp rdx, rcx
  cmp rdx, rdx
  cmp rdx, rbx

  CVTSI2SD xmm1, rax
  DIVPD xmm0, xmm1
  SQRTPD xmm0, xmm0
  MULPD xmm0, xmm1
  CVTSD2SI rax, xmm0
  ret
  mov rax, 17000
  CVTSI2SD xmm0, rax
  mov rax, 1000
  CVTSI2SD xmm1, rax
  DIVPD xmm0, xmm1
  SQRTPD xmm0, xmm0
  MULPD xmm0, xmm1
  CVTSD2SI rax, xmm0
  ret
  mov rax, 17000
  CVTSI2SD xmm0, rax
  mov rax, 1000
  CVTSI2SD xmm1, rax
  DIVPD xmm0, xmm1
  SQRTPD xmm0, xmm0
  MULPD xmm0, xmm1
  CVTSD2SI rax, xmm0
  ret
  mov rax, 17000
  CVTSI2SD xmm0, rax
  mov rax, 1000
  CVTSI2SD xmm1, rax
  DIVPD xmm0, xmm1
  SQRTPD xmm0, xmm0
  MULPD xmm0, xmm1
  CVTSD2SI rax, xmm0
  ret
  mov rax, 17000
  CVTSI2SD xmm0, rax
  mov rax, 1000
  CVTSI2SD xmm1, rax
  DIVPD xmm0, xmm1
  SQRTPD xmm0, xmm0
  MULPD xmm0, xmm1
  CVTSD2SI rax, xmm0
  ret
  mov rax, 17000
  CVTSI2SD xmm0, rax
  mov rax, 1000
  CVTSI2SD xmm1, rax
  DIVPD xmm0, xmm1
  SQRTPD xmm0, xmm0
  MULPD xmm0, xmm1
  CVTSD2SI rax, xmm0
  ret
  mov rax, 17000
  CVTSI2SD xmm0, rax
  mov rax, 1000
  CVTSI2SD xmm1, rax
  DIVPD xmm0, xmm1
  SQRTPD xmm0, xmm0
  MULPD xmm0, xmm1
  CVTSD2SI rax, xmm0
  ret
  mov rax, 17000
  CVTSI2SD xmm0, rax
  mov rax, 1000
  CVTSI2SD xmm1, rax
  DIVPD xmm0, xmm1
  SQRTPD xmm0, xmm0
  MULPD xmm0, xmm1
  CVTSD2SI rax, xmm0
  ret
  mov rax, 17000
  CVTSI2SD xmm0, rax
  mov rax, 1000
  CVTSI2SD xmm1, rax
  DIVPD xmm0, xmm1
  SQRTPD xmm0, xmm0
  MULPD xmm0, xmm1
  CVTSD2SI rax, xmm0
  ret
  mov rax, 17000
  CVTSI2SD xmm0, rax
  mov rax, 1000
  CVTSI2SD xmm1, rax
  DIVPD xmm0, xmm1
  SQRTPD xmm0, xmm0
  MULPD xmm0, xmm1
  CVTSD2SI rax, xmm0
  ret
  mov rax, 17000
  CVTSI2SD xmm0, rax
  mov rax, 1000
  CVTSI2SD xmm1, rax
  DIVPD xmm0, xmm1
  SQRTPD xmm0, xmm0
  MULPD xmm0, xmm1
  CVTSD2SI rax, xmm0
  ret
  mov rax, 17000
  CVTSI2SD xmm0, rax
  mov rax, 1000
  CVTSI2SD xmm1, rax
  DIVPD xmm0, xmm1
  SQRTPD xmm0, xmm0
  MULPD xmm0, xmm1
  CVTSD2SI rax, xmm0
  ret
  mov rax, 17000
  CVTSI2SD xmm0, rax
  mov rax, 1000
  CVTSI2SD xmm1, rax
  DIVPD xmm0, xmm1
  SQRTPD xmm0, xmm0
  MULPD xmm0, xmm1
  CVTSD2SI rax, xmm0
  ret
  cmp rax, rax
  je hui
  jne hui
  jg hui
  jge hui
  jl hui
  jle hui
  call hui
