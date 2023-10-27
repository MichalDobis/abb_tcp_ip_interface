MODULE MainModule
VAR robtarget home_pose10:=[[1141.03,53.46,524.22],[0.029753,-0.39941,-0.916272,-0.00574422],[0,0,0,0],[9E+09,9E+09,9E+09,9E+09,9E+09,9E+09]];

FUNC num moveJ_with_err_handle(robtarget target)
    VAR jointtarget jointcheck;
    
    jointcheck := CalcJointT(target, tool0, \WObj:=wobj0);
    MoveJ target, v500, z30, tool0;
    RETURN 0;
    ERROR
    TPWrite "ERROR: " + NumToStr(ERRNO,0);
    RETURN ERRNO;
ENDFUNC

FUNC num moveL_with_err_handle(robtarget target)
    VAR jointtarget jointcheck;
    
    jointcheck := CalcJointT(target, tool0, \WObj:=wobj0);
    MoveL target, v500, z30, tool0;
    RETURN 0;
    ERROR
    TPWrite "ERROR: " + NumToStr(ERRNO,0);
    RETURN ERRNO;
ENDFUNC

PROC main()
    VAR num current_cmd:=0;
    VAR num err_code:=0;
    
    MoveJ home_pose10, v500, z30, tool0;
	init_server 11003;
	wait_for_client;
    
	WHILE (TRUE) DO
	    current_cmd:= wait_for_command();
        TEST current_cmd
		    CASE MOVE_J:
		         err_code:=moveJ_with_err_handle(recv_target());
		         send_result(err_code);
		    CASE MOVE_L:
		         err_code:=moveL_with_err_handle(recv_target());
		         send_result(err_code);
		    CASE CURRENT_POSE:
		        send_position(CRobT(\TaskName:="T_ROB1" \Tool:=tool0 \WObj:=wobj0));
		    DEFAULT:
		        TPWrite "Undefined command...";
		        ExitCycle;
		ENDTEST
	ENDWHILE
    
ENDPROC

ENDMODULE
