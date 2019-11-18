;;
;; program to test nested ifs
;;

defun testIfs ;; called with one parameter n

arg1           ; get n to the top of the stack

dup 16 gt
if   ; 16 > n

    dup 8 gt
    if	; 8 > n

    	dup 4 gt
	if	; 4 > n
	    0
	else		; 4 <= n
	    1
	endif

    else		; 8 <= n
       2
    endif

else		; 16 <= n

     dup 24 gt
     if	; 24 > n

     	dup 20 gt
	if	; 20 > n
           3
        else   	  	; 20 <= n
           4
	endif

     else   	  	; 24 <= n

        dup 32 gt
        if	; 32 > n
           5
	else
	    -10
        endif

     endif

endif

return


defun main 
5 testIfs printnum endl
11 testIfs printnum endl
28 testIfs printnum endl
35 testIfs printnum endl
return