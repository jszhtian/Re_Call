SCRP   �N  �N  ��RIQS   TRAP  .   media/script/nut/0010_10eup_�`���[�g���A��.nut     mainTRAP                    
      TRAP     main     endfile     sceneTRAP     thisTRAPTRAP     this        	   TRAP             +         	   TRAPTRAP          0              0              0          �  TRAPTRAP  .   media/script/nut/0010_10eup_�`���[�g���A��.nut     mainTRAP                           TRAP     MainInit     ScarGet  	   吸血鬼     喉が渇く     SetEuphoriaSTTutorial     QuickMenuDisable     EuphoriaSet     Route     Towa_Clothes     コート_通常     EuphoriaStart     SetRandomVoiceArray  
   eup_client     GetCheckReadFile     scene     endfileTRAP     thisTRAPTRAP     this           TRAP                          	         
                                       TRAPTRAP                                                  	     	   
                                                 �  TRAP     TRAP  .   media/script/nut/0010_10eup_�`���[�g���A��.nut     endfileTRAP             
              TRAP     EuphoriaEnd     PalletteSelect     SetEuphoriaSTTutorial  	   吸血鬼           ResetCharacter     QuickMenuEnable     PreGameName     GameName     NextGameName     MainEndTRAP     thisTRAPTRAP     this           TRAP                           !      "   
   %      &      (      )      TRAPTRAP                                                              	         
           �  TRAP     TRAP  .   media/script/nut/0010_10eup_�`���[�g���A��.nut     sceneTRAP�             �       i     TRAP  	   SceneInit           PrintGO  	   上背景     Clear     endA     endB     endC     endD     CreateColorSP  	   絵色黒     TUTORIAL_PRIORITY     BLACK     CreateSprite     BgCopy02     Center     Middle     SCREEN     SetShadingPower     SHADE_LEVE_LOW     絵効果00     cg/sys/dialog/bg_colorBAR.png     Scroll     Wait     CreateEuphoriaSound  
   bgm040_eup     StSetColorBg     bg     CreateTextureSP     絵EV00  '   cg/ev/ev4000_80_吸血鬼euphoria_a.png     BLEND_MODE_NORMAL  
   CreateMask     絵覆      cg/pp/eup/ppEV用マスク_a.png     Rotate     絵EV01  @   cg2/pp/4000_80eup_吸血鬼/ppev4000_80_吸血鬼euphoria_a2.png     SetMask     絵EV02  @   cg2/pp/4000_80eup_吸血鬼/ppev4000_80_吸血鬼euphoria_a3.png     g_ScriptManager     get     BootFile     CreateSound  
   bgm035a_sp     SOUND_TYPE_BGM     sound/bgm/bgm035a_sp.flac     SetParameter  	   classname  	   SetVolume     SetLoopPoint  
   FadeDelete     Dxl1     Route     PreParameuphoria     PreParammad  
   SetInitial  	   吸血鬼  
   SetPurpose  E   こいつと話をして、隠している欲望を暴いてみるか     SetBorderlineEuphoria     SetBorderlineMad     SetClearlineEuphoria     SetClearlineMad  	   MakeReset  
   MakeTarget     EuphoriaUIDisable     StartTutorial  #   0010_10eup_チュートリアル_01     EuphoriaSoundPlay     CreatePlainSP  	   絵板写     Delete     BgCopy     絵EV*     SetColor     CreateSE     EuphoriaIn_SE01     se物体_鎖_鳴る03  
   MusicStart     EuphoriaIn_Link     EuphoriaIn_01     AxlDxl1d     EuphoriaIn_02     Fade     EuphoriaIn_03     CreateProcess     EuphoriaIn_Pro     Request     Start     Disused     EuphoriaUIEnable     WaitEuphoriaUILoop  
   WaitAction     _EuphoriaProcess/first_pro_*     EuphoriaIn_*  
   EuphoriaIn     FadeBuSmoke     St     C  #   bu吸血鬼_コート_通常_normal     FadeSt     Status     skip  
   ToggleSkip     auto     ToggleAutoText  #   0010_10eup_チュートリアル_02     FwG     fwトワ_コート_通常_cool     Nm     nmトワ  	   TypeBegin     Print  �   
//【トワ】
<PARTVOICE='トワ,トワ,voice/70/1103240a01'>
<voice name='トワ' class='トワ' src='voice/eup/4000_80eup_吸血鬼/0010'>
「最近、噂になってる吸血鬼ってお前のことだよな」
  $   bu吸血鬼_コート_通常_trouble     nm吸血鬼a  �   
//【吸血鬼】
<PARTVOICE='吸血鬼,吸血鬼,voice/70/6101420c11'>
<voice name='吸血鬼' class='吸血鬼' src='voice/eup/4000_80eup_吸血鬼/0020'>
「……俺は、吸血鬼じゃ……ない」
     TextBoxDelete  	   MakeRefer  *   酷く苦しそうにしてるな……。     MakeNegativeOn     詰めるか。     MakePositiveOn     冷静に返すか。     MakeShowMenu     ネガ  �   
//【トワ】
<PARTVOICE='トワ,トワ,voice/70/1101110a01'>
<voice name='トワ' class='トワ' src='voice/eup/4000_80eup_吸血鬼/0030'>
「血を飲みたがってるのに？」
  %   bu吸血鬼_コート_通常_trouble2  �   
//【吸血鬼】
//<PARTVOICE='吸血鬼,吸血鬼,voice/70/6101020c11'>
<PARTVOICE='吸血鬼,吸血鬼,voice/70/6101590c11'>
<voice name='吸血鬼' class='吸血鬼' src='voice/eup/4000_80eup_吸血鬼/0040'>
「……っ」
     MakeCommitParam     ポジ      fwトワ_コート_通常_normal  
  
//【トワ】
//<PARTVOICE='トワ,トワ,voice/70/1101880a01'>
<PARTVOICE='トワ,トワ,voice/70/1101210a01'>
<voice name='トワ' class='トワ' src='voice/eup/4000_80eup_吸血鬼/0050'>
「お前が否定しても、他の人間はそう思ってる」
  �   
//【吸血鬼】
//<PARTVOICE='吸血鬼,吸血鬼,voice/70/6101070c11'>
<PARTVOICE='吸血鬼,吸血鬼,voice/70/6100100c11'>
<voice name='吸血鬼' class='吸血鬼' src='voice/eup/4000_80eup_吸血鬼/0060'>
「…………」
     ……誘導するか。     MakePaletteOnTutorial  -   アタシもそういうの飲みたいわー     飲みたいんだろ？  3   でもとりあえず今は、つらいっす……     つらいんじゃないのか     fwトワ_コート_通常_cool2  �   
//【トワ】
<PARTVOICE='トワ,トワ,voice/70/1100350a01'>
<voice name='トワ' class='トワ' src='voice/eup/4000_80eup_吸血鬼/0070'>
「飲みたいんだろ？」
  %   bu吸血鬼_コート_通常_surprise  �   
<PARTVOICE='吸血鬼,吸血鬼,voice/70/6101650c11'>
//【吸血鬼】
<voice name='吸血鬼' class='吸血鬼' src='voice/eup/4000_80eup_吸血鬼/0080'>
「違う……」
     fwトワ_コート_通常_sigh  �   
//【トワ】
<PARTVOICE='トワ,トワ,voice/70/1103750a01'>
<voice name='トワ' class='トワ' src='voice/eup/4000_80eup_吸血鬼/0090'>
「つらいんじゃないのか」
  �   
//【吸血鬼】
<PARTVOICE='吸血鬼,吸血鬼,voice/70/6101690c11'>
<voice name='吸血鬼' class='吸血鬼' src='voice/eup/4000_80eup_吸血鬼/0100'>
「……うるさい」
  #   0010_10eup_チュートリアル_05     CreateTextureEX     絵EV03  ?   cg2/pp/4000_80eup_吸血鬼/ppev4000_80_吸血鬼euphoria_b.png     EuphoriaHideUI     DeleteBuSmoke     StT     now     絵板写02     EuphoriaShowUI  $   bu吸血鬼_コート_通常_serious     Draw  $   この男の「願望」は……。     MakeWordOnTutorial     喉が渇く     voice/90/6100070a01     SetErase     C_*     生稲     絵EV0*     DeleteEuphoriaUI  #   0010_10eup_チュートリアル_07     SetVolumeEX     bgm*     SE*     voice*     ClearFadeAll     NextGameName     0010_20_プロローグ.nut     flgeup  $   0010_10eup_チュートリアル.nut     passing     SceneEndTRAP     thisTRAPTRAP     代金_初回   �  g       代金   �  g       吸血鬼_初回   �  g    	   吸血鬼   �  g    	   bonus_mad   �   g    	   bonus_eup
   �   g       voicewaittime	      g       route   
   g       lockpalette02   	   g       lockpalette01      g       paletteopen02      g       paletteopen01      g       result_word      g       result_palette      g       result_negapoti      g       this        h  TRAP+       -       4      5      6      7      8      9      :      ;   	   <   
   ?      A      B   &   C   -   D   6   E   ?   F   F   G   N   H   N   K   Q   L   T   M   W   O   b   P   h   Q   r   R      S   �   V   �   W   �   X   �   Y   �   [   �   \   �   ]   �   ^   �   d   �   e   �   h   �   i   �   k   �   l   �   m   �   n   �   p   �   q   �   s   �   w     x     z     ~     �     �     �     �     �     �      �   #  �   *  �   3  �   6  �   ?  �   I  �   S  �   ]  �   h  �   r  �   u  �     �   �  �   �  �   �  �   �  �   �  �   �  �   �  �   �  �   �  �   �  �   �  �   �  �   �  �   �  �   �  �   �  �   �  �   �  �   �  �   �  �   �  �   �  �   �  �   �  �   �  �     �     �     �     �      �   $  �   -  �   2  �   5  �   8  �   <  �   >  �   @  �   C  �   F  �   M  �   U    Y  	  [  
  ^    a    d    h    q    v    y    |  "  �  $  �  (  �  *  �  +  �  -  �  .  �  7  �  8  �  :  �  <  �  =  �  D  �  F  �  Q  �  R  �  S  �  T  �  U  �  V  �  W  �  X  �  k  �  m  �  n  �  p  �  q  �  y  �  z  �  |  �  ~  �    �  �  �  �  �  �  �  �  �  �  �  �    �    �    �    �    �    �    �     �  "  �  $  �  '  �  >  �  A  �  D  �  J  �  T  �  a  �  d  �  q  �  ~  �  �  �  �  �  �  �  �  �  �  �  �  �  �  �  �  �  �  �  �  �  �  �  �  �  �  �  �  �  �  �  �  �  �  �  �  �  �  �  �  �  �  �  �  �  �  �  �  �  �  �  �  �  �  �  �  �  �  �  �  �  �  �  �  �  �  �     �    �                       !    #    %  	  (  
  ?    B    E    K    Q    W    [    ^    `    f    h  TRAPTRAP                                                    �  	     
       N    
   
   	
     	

          

    +

    	     	              +    	     	              +    	     	              
     
     

  (   
  	   
 
                +   	  
   
   
                 +   	     	       
   
   
            �       	  d                
   
   
      l                     
   
   
                         -  
   
         
 �    
   
   
      
   
   
      
   
   
      n        	     	               �          
   

    
 !                 "     
   
#   
 !                 �                           
   
	   
 $     �     n        -  <        -  %             �          
   

&   
 $   !
   
   
 '     �             -  }        -  (             �          
   

)   	
  *   

+   	  
   
,   

-   .     /           
   
0   
 -   1     
   
2   
 -                            
   
3   
 -     P    N   
   
   	
     	

          

    +

    	     	              +    	     	              +    	     	              
     
     

     
  4   
      �    5   	       
   
      4   
      �    5   	       
   
6   	
  7   	

 6   	  8   	 9    :                   ;    <        =            -     >            -     ?            @            A       B    :           	     	              +    	     	              +    	     	              +    	     	              
     
     
  �     C           �           �       D    E                    F         �    �       G    H     �        I    !        I    J        I    K        L    H     �    �    �    �           H     �    �       	  d                  M    N   O   P    N           �          �                  #    Q     �                
                         	#    R     �                     S   	                	#    Q     �                                          	#    R     �                        -  S   	                	#    R     �                      S   	                 	    �       #    T     �          P           5   	                 	#    R     �          P           5   	                 	U    R     ,          5   	                     �        #    V     �                      5   	                 	#    T     �                      5   	                 	U    T     �    �    5   	                     �       W    X         0     Y    X   Z        Y    X   [        \       ]       ^    _        I    `                          F         �    �       a                �       b    :              c    d                  =           =  e        f    d     �                �       g   	  h   	               i             g   	  j   	               k                	     	              +    	     	              +    	     	              +    	     	              
     
     
       D    l        m    n        o    p        q             r         s        c    d                  =           =  t        f    d     ,             o    u        q             r    (     v        w       A       B    :        x    y        z    {                  +        |    }                  +        -     ~               
     �*     m    n        o    p        q             r    2     �        c    d                  =           =  �        f    d     ,             o    u        q             r    <     �        w       �       +      �   �)     m    �        o    p        q             r    F     �        c    d                  =           =  e        f    d     ,             o    u        q             r    P     �        w       �       w       A       B    :        x    �        �    �   �             +        -     �    �   �             +        ~                
  �   �*     m    �        o    p        q             r    Z     �        c    d                  =           =  �        f    d     ,             o    u        q             r    d     �        w       �       +      �   �)     m    �        o    p        q             r    n     �        c    d                  =           =  �        f    d     ,             o    u        q             r    x     �        w       �           �          	     	              +    	     	              +    	     	              +    	     	              
     
     
       D    �            �            !                 "        #    !                 �                              	    $     �     n        -  <        -  %             �             
&    $   !       '     �             -  }        -  (             �             
�    �     �     n        -  <        -  �                            
&    �   !   �       �    :              U    �   	  d   	 �   	 �          5   	                 4    H     �    5   	          G    �     �        I    $        U    �           �                           �       4    �     �    5   	              �       �       b    :              c    d                  =           =  �        f    d     �                                              �       A       B    :        x    �      �    �                 �           ~               
  �   �           ����   G    �     �       �       U    �   	  d   	 �   	                                 �    �             �    �              I    H        I    �        I    !        	    
     �       	              l                                                    -     �       4    �     �                 C       �           �          	     	              +    	     	              +    	     	              +    	     	              
     
     
       D    �            �       �    �     �                  �    �     �                  �    �     �                  �    �                �       �   �       	  �   	 �   	 �             �           �  TRAPTRAP  .   media/script/nut/0010_10eup_�`���[�g���A��.nut  TRAP	                    (       TRAP     Wait     Fade     EuphoriaIn_03     Dxl1     EuphoriaIn_02     Axl1     Scale     EuphoriaIn_Link     EuphoriaIn_*TRAP     thisTRAPTRAP     this        '   TRAP�       �       �      �      �      �      �      �   '   TRAPTRAP     d                 �    �       	                          ,             	                      �                �    �    �                   	              �             	                     �  TRAP
               LIAT    