MODULE MainModule
VAR robtarget home_pose10:=[[1141.03,53.46,524.22],[0.029753,-0.39941,-0.916272,-0.00574422],[0,0,0,0],[9E+09,9E+09,9E+09,9E+09,9E+09,9E+09]];
VAR robtarget new_target:=[[1141.03,53.46,524.22],[0.029753,-0.39941,-0.916272,-0.00574422],[0,0,0,0],[9E+09,9E+09,9E+09,9E+09,9E+09,9E+09]];

PROC main()
    VAR num current_cmd:=0;
    
    MoveJ home_pose10, v500, z30, tool0;
	init_server 11003;
	wait_for_client;
    
	WHILE (TRUE) DO
	    current_cmd:= wait_for_command();
        TEST current_cmd
		    CASE MOVE_J:
			     new_target := recv_target();
		         MoveJ new_target, v500, z30, tool0;
		         send_result(0);
		    CASE MOVE_L:
		         new_target := recv_target();
		         MoveL new_target, v500, z30, tool0;
		         send_result(0);
		    CASE CURRENT_POSE:
		        send_position(CRobT(\TaskName:="T_ROB1" \Tool:=tool0 \WObj:=wobj0));
		    DEFAULT:
		        TPWrite "Undefined command...";
		        ExitCycle;
		ENDTEST
	ENDWHILE
ENDPROC

ENDMODULE
