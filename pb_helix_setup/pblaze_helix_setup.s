;#!pblaze-cc source : pblaze_helix_setup.c
;#!pblaze-cc create : Thu Aug 22 14:13:52 2019
;#!pblaze-cc modify : Thu Aug 22 14:13:52 2019
;------------------------------------------------------------
address 0x000
boot:
  call init

;------------------------------------------------------------
;/home/barawn/pb_helix_setup/pblaze_helix_setup.c
loop:
 L_2b66c990c2afa22640098415295824a9_63:
 ;/home/barawn/pb_helix_setup/pblaze_helix_setup.c:81
  ;void loop (void)

 JOIN_4:
 ;/home/barawn/pb_helix_setup/pblaze_helix_setup.c:81
  ;endfunc

_end_loop:
  jump loop


;------------------------------------------------------------
;/home/barawn/pb_helix_setup/pblaze_helix_setup.c
init:
 L_2b66c990c2afa22640098415295824a9_0:
 ;/home/barawn/pb_helix_setup/pblaze_helix_setup.c:17
  ;void init (void)

 L_2b66c990c2afa22640098415295824a9_1:
 ;/home/barawn/pb_helix_setup/pblaze_helix_setup.c:19
      input s0, 128

 L_2b66c990c2afa22640098415295824a9_2:
 ;/home/barawn/pb_helix_setup/pblaze_helix_setup.c:20
      ;if (s0 |^ 2), L_2b66c990c2afa22640098415295824a9_3, L_2b66c990c2afa22640098415295824a9_20
      test s0, 2
      jump NZ, L_2b66c990c2afa22640098415295824a9_20

 L_2b66c990c2afa22640098415295824a9_3:
 ;/home/barawn/pb_helix_setup/pblaze_helix_setup.c:22
              outputk 0, 12
 ;/home/barawn/pb_helix_setup/pblaze_helix_setup.c:23
              outputk 0, 13
 ;/home/barawn/pb_helix_setup/pblaze_helix_setup.c:24
              outputk 1, 0

 L_2b66c990c2afa22640098415295824a9_4:
 ;/home/barawn/pb_helix_setup/pblaze_helix_setup.c:25
              ;do

 L_2b66c990c2afa22640098415295824a9_5:
 ;/home/barawn/pb_helix_setup/pblaze_helix_setup.c:27
                      input sA, 0

 L_2b66c990c2afa22640098415295824a9_6:
 ;/home/barawn/pb_helix_setup/pblaze_helix_setup.c:29
              ;dowhile (sA |^ 8), L_2b66c990c2afa22640098415295824a9_4, L_2b66c990c2afa22640098415295824a9_7
              test sA, 8
              jump Z, L_2b66c990c2afa22640098415295824a9_4

 L_2b66c990c2afa22640098415295824a9_7:
 ;/home/barawn/pb_helix_setup/pblaze_helix_setup.c:24
              input s1, 8
 ;/home/barawn/pb_helix_setup/pblaze_helix_setup.c:25
              input s0, 9
 ;/home/barawn/pb_helix_setup/pblaze_helix_setup.c:27
              and s1, 252

 L_2b66c990c2afa22640098415295824a9_8:
 ;/home/barawn/pb_helix_setup/pblaze_helix_setup.c:29
              ;if (s0 & 240), L_2b66c990c2afa22640098415295824a9_9, L_2b66c990c2afa22640098415295824a9_9
              test s0, 240
              jump Z, L_2b66c990c2afa22640098415295824a9_9
              jump NZ, L_2b66c990c2afa22640098415295824a9_9

 JOIN_6:
 ;/home/barawn/pb_helix_setup/pblaze_helix_setup.c:29
              ;endif of L_2b66c990c2afa22640098415295824a9_8

 L_2b66c990c2afa22640098415295824a9_9:
 ;/home/barawn/pb_helix_setup/pblaze_helix_setup.c:32
              move s3, 0
              move s2, 0
 ;/home/barawn/pb_helix_setup/pblaze_helix_setup.c:33
              move s4, 0
 ;/home/barawn/pb_helix_setup/pblaze_helix_setup.c:35
              move sA, 208
 ;/home/barawn/pb_helix_setup/pblaze_helix_setup.c:36
              move sB, 1
 ;/home/barawn/pb_helix_setup/pblaze_helix_setup.c:37
              move sC, 0
 ;/home/barawn/pb_helix_setup/pblaze_helix_setup.c:38
              call i2c_writereg_fn
 ;/home/barawn/pb_helix_setup/pblaze_helix_setup.c:37
              move sA, 1
 ;/home/barawn/pb_helix_setup/pblaze_helix_setup.c:38
              call delay1us_fn

 L_2b66c990c2afa22640098415295824a9_10:
 ;/home/barawn/pb_helix_setup/pblaze_helix_setup.c:43
              ;do

 L_2b66c990c2afa22640098415295824a9_11:
 ;/home/barawn/pb_helix_setup/pblaze_helix_setup.c:45
                      add s3, 4
                      addcy s2, 0 ;/home/barawn/pb_helix_setup/pblaze_helix_setup.c:47
                      output s3, 12
 ;/home/barawn/pb_helix_setup/pblaze_helix_setup.c:48
                      output s2, 13
 ;/home/barawn/pb_helix_setup/pblaze_helix_setup.c:49
                      outputk 1, 0

 L_2b66c990c2afa22640098415295824a9_12:
 ;/home/barawn/pb_helix_setup/pblaze_helix_setup.c:50
                      ;do

 L_2b66c990c2afa22640098415295824a9_13:
 ;/home/barawn/pb_helix_setup/pblaze_helix_setup.c:52
                              input sA, 0

 L_2b66c990c2afa22640098415295824a9_14:
 ;/home/barawn/pb_helix_setup/pblaze_helix_setup.c:54
                      ;dowhile (sA |^ 8), L_2b66c990c2afa22640098415295824a9_12, L_2b66c990c2afa22640098415295824a9_15
                      test sA, 8
                      jump Z, L_2b66c990c2afa22640098415295824a9_12

 L_2b66c990c2afa22640098415295824a9_15:
 ;/home/barawn/pb_helix_setup/pblaze_helix_setup.c:50
                      move sA, 1
 ;/home/barawn/pb_helix_setup/pblaze_helix_setup.c:51
                      call delay1us_fn
 ;/home/barawn/pb_helix_setup/pblaze_helix_setup.c:52
                      input sA, 11
 ;/home/barawn/pb_helix_setup/pblaze_helix_setup.c:54
                      call delay3ms_fn
 ;/home/barawn/pb_helix_setup/pblaze_helix_setup.c:56
                      input s5, 8
 ;/home/barawn/pb_helix_setup/pblaze_helix_setup.c:58
                      input s6, 9
 ;/home/barawn/pb_helix_setup/pblaze_helix_setup.c:64
                      input sC, 10

 L_2b66c990c2afa22640098415295824a9_16:
 ;/home/barawn/pb_helix_setup/pblaze_helix_setup.c:66
                      ;if (sC != s4), L_2b66c990c2afa22640098415295824a9_17, L_2b66c990c2afa22640098415295824a9_18
                      compare sC, s4
                      jump Z, L_2b66c990c2afa22640098415295824a9_18

 L_2b66c990c2afa22640098415295824a9_17:
 ;/home/barawn/pb_helix_setup/pblaze_helix_setup.c:68
                              move sA, 208
 ;/home/barawn/pb_helix_setup/pblaze_helix_setup.c:69
                              move sB, 1
 ;/home/barawn/pb_helix_setup/pblaze_helix_setup.c:70
                              call i2c_writereg_fn
 ;/home/barawn/pb_helix_setup/pblaze_helix_setup.c:69
                              move s4, sC

 JOIN_7:
 ;/home/barawn/pb_helix_setup/pblaze_helix_setup.c:66
                      ;endif of L_2b66c990c2afa22640098415295824a9_16

 L_2b66c990c2afa22640098415295824a9_18:
 ;/home/barawn/pb_helix_setup/pblaze_helix_setup.c:72
                      move sA, 208
 ;/home/barawn/pb_helix_setup/pblaze_helix_setup.c:73
                      move sB, s6
 ;/home/barawn/pb_helix_setup/pblaze_helix_setup.c:74
                      move sC, s5
 ;/home/barawn/pb_helix_setup/pblaze_helix_setup.c:75
                      call i2c_writereg_fn
 ;/home/barawn/pb_helix_setup/pblaze_helix_setup.c:73
                      move sA, 1
 ;/home/barawn/pb_helix_setup/pblaze_helix_setup.c:74
                      call delay1us_fn

 L_2b66c990c2afa22640098415295824a9_19:
 ;/home/barawn/pb_helix_setup/pblaze_helix_setup.c:75
              ;dowhile (s2.s3 != s0.s1), L_2b66c990c2afa22640098415295824a9_10, JOIN_5
              compare s3, s1
              comparecy s2, s0

              jump NZ, L_2b66c990c2afa22640098415295824a9_10

 JOIN_5:
 ;/home/barawn/pb_helix_setup/pblaze_helix_setup.c:20
      ;endif of L_2b66c990c2afa22640098415295824a9_2

 L_2b66c990c2afa22640098415295824a9_20:
 ;/home/barawn/pb_helix_setup/pblaze_helix_setup.c:77
      move s0, 2
 ;/home/barawn/pb_helix_setup/pblaze_helix_setup.c:78
      output s0, 128

 JOIN_0:
 ;/home/barawn/pb_helix_setup/pblaze_helix_setup.c:17
  ;endfunc

_end_init:
  return


;------------------------------------------------------------
;pb_delay.h
delay3ms_fn:
 L_aec9c009f1bf6fcfdca40011ba214682_21:
 ;pb_delay.h:7
  ;void delay3ms_fn ()

 L_aec9c009f1bf6fcfdca40011ba214682_22:
 ;pb_delay.h:8
      ;while (sA != 0), L_aec9c009f1bf6fcfdca40011ba214682_23, JOIN_1
      compare sA, 0
      jump Z, JOIN_1

 L_aec9c009f1bf6fcfdca40011ba214682_23:
 ;pb_delay.h:10
              move sC, 80
              move sB, 195

 L_aec9c009f1bf6fcfdca40011ba214682_24:
 ;pb_delay.h:11
              ;do

 L_aec9c009f1bf6fcfdca40011ba214682_25:
 ;pb_delay.h:12
                      sub sC, 1
                      subcy sB, 0
 L_aec9c009f1bf6fcfdca40011ba214682_26:
 ;pb_delay.h:14
              ;dowhile (Z == 0), L_aec9c009f1bf6fcfdca40011ba214682_24, L_aec9c009f1bf6fcfdca40011ba214682_27
            
              jump NZ, L_aec9c009f1bf6fcfdca40011ba214682_24

 L_aec9c009f1bf6fcfdca40011ba214682_27:
 ;pb_delay.h:14
              sub sA, 1

 JOIN_8:
 ;pb_delay.h:8
      ;endwhile
      jump L_aec9c009f1bf6fcfdca40011ba214682_22

 JOIN_1:
 ;pb_delay.h:7
  ;endfunc

_end_delay3ms_fn:
  return


;------------------------------------------------------------
;pb_delay.h
delay1us_fn:
 L_aec9c009f1bf6fcfdca40011ba214682_28:
 ;pb_delay.h:33
  ;void delay1us_fn ()

 L_aec9c009f1bf6fcfdca40011ba214682_29:
 ;pb_delay.h:34
      ;while (sA != 0), L_aec9c009f1bf6fcfdca40011ba214682_30, JOIN_2
      compare sA, 0
      jump Z, JOIN_2

 L_aec9c009f1bf6fcfdca40011ba214682_30:
 ;pb_delay.h:35
              move sB, 23

 L_aec9c009f1bf6fcfdca40011ba214682_31:
 ;pb_delay.h:36
              ;do

 L_aec9c009f1bf6fcfdca40011ba214682_32:
 ;pb_delay.h:37
                      sub sB, 1

 L_aec9c009f1bf6fcfdca40011ba214682_33:
 ;pb_delay.h:39
              ;dowhile (Z == 0), L_aec9c009f1bf6fcfdca40011ba214682_31, L_aec9c009f1bf6fcfdca40011ba214682_34
            
              jump NZ, L_aec9c009f1bf6fcfdca40011ba214682_31

 L_aec9c009f1bf6fcfdca40011ba214682_34:
 ;pb_delay.h:39
              sub sA, 1

 JOIN_9:
 ;pb_delay.h:34
      ;endwhile
      jump L_aec9c009f1bf6fcfdca40011ba214682_29

 JOIN_2:
 ;pb_delay.h:33
  ;endfunc

_end_delay1us_fn:
  return


;------------------------------------------------------------
;pb_axi_iic.h
i2c_writereg_fn:
 L_a06a54f5a3773766f2a8f8795230bb48_35:
 ;pb_axi_iic.h:30
  ;void i2c_writereg_fn (void)

 L_a06a54f5a3773766f2a8f8795230bb48_36:
 ;pb_axi_iic.h:37
      outputk 128, 0
 ;pb_axi_iic.h:38
      outputk 10, 8
 ;pb_axi_iic.h:39
      outputk 64, 12
 ;pb_axi_iic.h:40
      outputk 16, 13
 ;pb_axi_iic.h:41
      outputk 3, 0

 L_a06a54f5a3773766f2a8f8795230bb48_37:
 ;pb_axi_iic.h:42
      ;do

 L_a06a54f5a3773766f2a8f8795230bb48_38:
 ;pb_axi_iic.h:44
              input sD, 0

 L_a06a54f5a3773766f2a8f8795230bb48_39:
 ;pb_axi_iic.h:46
      ;dowhile (sD |^ 8), L_a06a54f5a3773766f2a8f8795230bb48_37, L_a06a54f5a3773766f2a8f8795230bb48_40
      test sD, 8
      jump Z, L_a06a54f5a3773766f2a8f8795230bb48_37

 L_a06a54f5a3773766f2a8f8795230bb48_40:
 ;pb_axi_iic.h:47
      outputk 1, 8
 ;pb_axi_iic.h:48
      outputk 0, 12
 ;pb_axi_iic.h:49
      outputk 17, 13
 ;pb_axi_iic.h:50
      outputk 3, 0

 L_a06a54f5a3773766f2a8f8795230bb48_41:
 ;pb_axi_iic.h:51
      ;do

 L_a06a54f5a3773766f2a8f8795230bb48_42:
 ;pb_axi_iic.h:53
              input sD, 0

 L_a06a54f5a3773766f2a8f8795230bb48_43:
 ;pb_axi_iic.h:55
      ;dowhile (sD |^ 8), L_a06a54f5a3773766f2a8f8795230bb48_41, L_a06a54f5a3773766f2a8f8795230bb48_44
      test sD, 8
      jump Z, L_a06a54f5a3773766f2a8f8795230bb48_41

 L_a06a54f5a3773766f2a8f8795230bb48_44:
 ;pb_axi_iic.h:39
      outputk 1, 9
 ;pb_axi_iic.h:40
      output sA, 8
 ;pb_axi_iic.h:41
      outputk 8, 12
 ;pb_axi_iic.h:42
      outputk 17, 13
 ;pb_axi_iic.h:43
      outputk 3, 0

 L_a06a54f5a3773766f2a8f8795230bb48_45:
 ;pb_axi_iic.h:44
      ;do

 L_a06a54f5a3773766f2a8f8795230bb48_46:
 ;pb_axi_iic.h:46
              input sD, 0

 L_a06a54f5a3773766f2a8f8795230bb48_47:
 ;pb_axi_iic.h:48
      ;dowhile (sD |^ 8), L_a06a54f5a3773766f2a8f8795230bb48_45, L_a06a54f5a3773766f2a8f8795230bb48_48
      test sD, 8
      jump Z, L_a06a54f5a3773766f2a8f8795230bb48_45

 L_a06a54f5a3773766f2a8f8795230bb48_48:
 ;pb_axi_iic.h:43
      outputk 0, 9
 ;pb_axi_iic.h:44
      output sB, 8
 ;pb_axi_iic.h:45
      outputk 3, 0

 L_a06a54f5a3773766f2a8f8795230bb48_49:
 ;pb_axi_iic.h:46
      ;do

 L_a06a54f5a3773766f2a8f8795230bb48_50:
 ;pb_axi_iic.h:48
              input sD, 0

 L_a06a54f5a3773766f2a8f8795230bb48_51:
 ;pb_axi_iic.h:50
      ;dowhile (sD |^ 8), L_a06a54f5a3773766f2a8f8795230bb48_49, L_a06a54f5a3773766f2a8f8795230bb48_52
      test sD, 8
      jump Z, L_a06a54f5a3773766f2a8f8795230bb48_49

 L_a06a54f5a3773766f2a8f8795230bb48_52:
 ;pb_axi_iic.h:47
      outputk 2, 9
 ;pb_axi_iic.h:48
      output sC, 8
 ;pb_axi_iic.h:49
      outputk 3, 0

 L_a06a54f5a3773766f2a8f8795230bb48_53:
 ;pb_axi_iic.h:50
      ;do

 L_a06a54f5a3773766f2a8f8795230bb48_54:
 ;pb_axi_iic.h:52
              input sD, 0

 L_a06a54f5a3773766f2a8f8795230bb48_55:
 ;pb_axi_iic.h:54
      ;dowhile (sD |^ 8), L_a06a54f5a3773766f2a8f8795230bb48_53, L_a06a54f5a3773766f2a8f8795230bb48_56
      test sD, 8
      jump Z, L_a06a54f5a3773766f2a8f8795230bb48_53

 L_a06a54f5a3773766f2a8f8795230bb48_56:
 ;pb_axi_iic.h:51
      ;do

 L_a06a54f5a3773766f2a8f8795230bb48_57:
 ;pb_axi_iic.h:52
              outputk 4, 12
 ;pb_axi_iic.h:53
              outputk 17, 13
 ;pb_axi_iic.h:54
              outputk 1, 0

 L_a06a54f5a3773766f2a8f8795230bb48_58:
 ;pb_axi_iic.h:55
              ;do

 L_a06a54f5a3773766f2a8f8795230bb48_59:
 ;pb_axi_iic.h:57
                      input sD, 0

 L_a06a54f5a3773766f2a8f8795230bb48_60:
 ;pb_axi_iic.h:59
              ;dowhile (sD |^ 8), L_a06a54f5a3773766f2a8f8795230bb48_58, L_a06a54f5a3773766f2a8f8795230bb48_61
              test sD, 8
              jump Z, L_a06a54f5a3773766f2a8f8795230bb48_58

 L_a06a54f5a3773766f2a8f8795230bb48_61:
 ;pb_axi_iic.h:53
              input sA, 8
 ;pb_axi_iic.h:57
              xor sA, 128

 L_a06a54f5a3773766f2a8f8795230bb48_62:
 ;pb_axi_iic.h:60
      ;dowhile (sA & 132), L_a06a54f5a3773766f2a8f8795230bb48_56, JOIN_3
      test sA, 132
      jump NZ, L_a06a54f5a3773766f2a8f8795230bb48_56

 JOIN_3:
 ;pb_axi_iic.h:30
  ;endfunc

_end_i2c_writereg_fn:
  return



;ISR
