@if exp="typeof(global.zoomrot_object) == 'undefined'"
@iscript

/*
	�w�i/�O�i�摜�̊g��k�����ʂɂ��\�����s���v���O�C��
*/

class ZoomRotPlugin extends KAGPlugin
{
	var cx, cy;                        // ��]���S
	var sr, ss, sx, sy;                // �����ʒu
	var dr, ds, dx, dy;                // �ŏI�ʒu
	var m00, m01, m10, m11, mtx, mty;  // �A�t�B���ϊ��s��p�����[�^
	var startTick;                     // �J�n�e�B�b�N
	var time;                          // �Y�[�����s���Ă��鎞��
	var accel;                         // �����x�I�ȓ������s���� ( �� : 0 : �� )
	var storage;                       // �摜��
	var key;                           // �J���[�L�[
	var moving = false;                // ���쒆�t���O
	var nextstop;                      // �����~�t���O
	var moveFunc;                      // �ړ��ʒu�v�Z�p�֐�
	var targetLayerName;               // �Ώۃ��C����
	var targetLayer;                   // �Ώۃ��C��
	var targetPage;                    // �Ώۃy�[�W�i�\�^����ʁj
	var tempLayer;                     // �e���|�������C��

	function ZoomRotPlugin(window)
	{
		super.KAGPlugin();
		this.window = window;
	}

	function finalize()
	{
		// finalize ���\�b�h
		// ���̃N���X�̊Ǘ����邷�ׂẴI�u�W�F�N�g�𖾎��I�ɔj��
		stop();

		invalidate tempLayer if tempLayer !== void;

		super.finalize(...);
	}

	function startZoomRot(
		storage, key, layer='0', page='fore',
		cx='center', cy='center',
		sr='0.0', ss='1.0', sx='center', sy='center',
		dr='0.0', ds='1.0', dx='center', dy='center',
		time='1000', accel='1.0' )
	{
		// storage : �\���摜
		// layer : �Ώۃ��C��
		// cx, cy : ��]���S�ʒu�i�摜��j
		// sr ss sx sy : �����ʒu
		// dr ds dy dy : �ŏI�ʒu
		// time : �Y�[�����s���Ă��鎞��
		// accel : ���������邩�ǂ���

		var base = window.fore.base;

		// �����̓�����~
		stop();

		// �e���|�������C�����m��
		{
			this.storage = storage;
			if( key == '' )
				this.key = clNone;
			else if( key == 'adapt' )
				this.key = clAdapt;
			else if( (key[0] == '0') && (key[0] == 'x') )
				this.key = +key;
			else
				this.key = (+key) + 0x3000000;
			
			if(tempLayer === void)
			{
				tempLayer = new Layer(window, base);
				tempLayer.loadImages(this.storage, clNone);
			}
		}

		// �Ώۃ��C����ݒ�
		{
			this.targetLayerName = layer;
			this.targetPage = page;
			if( layer == 'base')
			{
				if( page == 'back' )
					this.targetLayer = window.back.base;
				else
					this.targetLayer = window.fore.base;
			}
			else
			{
				if( page == 'back' )
					this.targetLayer = window.back.layers[+layer];
				else
					this.targetLayer = window.fore.layers[+layer];
				
				targetLayer.face = dfBoth;
				targetLayer.type = ltTransparent;
				targetLayer.setImageSize( base.imageWidth, base.imageHeight );
				targetLayer.setPos( 0,0, base.imageWidth,base.imageHeight );
			}
		}

		// �I�u�W�F�N�g�Ƀp�����[�^��ݒ�
		{
			// ��]���S
			if( (cx == '') || (cx == 'center') )
				this.cx = tempLayer.imageWidth / 2;
			else if( typeof +cx == 'Real' )
				this.cx = tempLayer.imageWidth * +cx;
			else
				this.cx = (real)cx;
			
			if( (cy == '') || (cy == 'center') )
				this.cy = tempLayer.imageHeight / 2;
			else if( typeof +cy == 'Real' )
				this.cy = tempLayer.imageHeight * +cy;
			else
				this.cy = (real)cy;
			
			// �����ʒu
			if( sr == '' )
				this.sr = (real)0;
			else if( typeof +sr == 'Real' )
				this.sr = Math.PI * +sr;
			else
				this.sr = (Math.PI/180) * +sr;
			
			if( ss == '' )
				this.ss = (real)1;
			else
				this.ss = (real)ss;
			
			if( (sx == '') || (sx == 'center') )
				this.sx = targetLayer.imageWidth / 2;
			else if( typeof +sx == 'Real' )
				this.sx = targetLayer.imageWidth * +sx;
			else
				this.sx = (real)sx;
			
			if( (sy == '') || (sy == 'center') )
				this.sy = targetLayer.imageHeight / 2;
			else if( typeof +sy == 'Real' )
				this.sy = targetLayer.imageHeight * +sy;
			else
				this.sy = (real)sy;
			
			// �ŏI�ʒu
			if( dr == '' )
				this.dr = (real)0;
			else if( typeof +dr == 'Real' )
				this.dr = Math.PI * +dr;
			else
				this.dr = (Math.PI/180) * +dr;
			
			if( ss == '' )
				this.ds = (real)1;
			else
				this.ds = (real)ds;
			
			if( (dx == '') || (dx == 'center') )
				this.dx = targetLayer.imageWidth / 2;
			else if( typeof +dx == 'Real' )
				this.dx = targetLayer.imageWidth * +dx;
			else
				this.dx = (real)dx;
			
			if( (dy == '') || (dy == 'center') )
				this.dy = targetLayer.imageHeight / 2;
			else if( typeof +dy == 'Real' )
				this.dy = targetLayer.imageHeight * +dy;
			else
				this.dy = (real)dy;
			
			// ���Ԑݒ�
			if( time == '' )
				this.time = 1000;
			else
				this.time = (int)time;
			
			if( accel == '' )
				this.accel = (real)0;
			else
				this.accel = (real)accel;
		}
		
		// �ړ��ʒu�v�Z�֐��̐ݒ�
		moveFunc = defaultMover;

		// �����ʒu�ɕ\��
		moveFunc(moveAt, 0);
		targetLayer.visible = true;

		// �J�n
		startTick = System.getTickCount();
		System.addContinuousHandler(continuousHandler);
		moving = true;
		nextstop = false;
	}

	function moveAt( m00, m01, m10, m11, myx, mty )
	{
		// m00 m01 m10 m11 mtx mty �ʒu�Ɉړ�

		targetLayer.visible = true;
		// ���C������x�N���A����
		targetLayer.fillRect( 0,0, targetLayer.imageWidth,targetLayer.imageHeight, 0 );
		// �A�t�B���ϊ��]��
		targetLayer.affineCopy(
				tempLayer, 0, 0, tempLayer.imageWidth, tempLayer.imageHeight, 'true',
				m00, m01, m10, m11, mtx, mty, 'stNearest'
			);
	}

	/*static*/ function defaultMover(func, tm)
	{
		// �ʒu�v�Z
		// tm �� 0.0(�J�n�_) �` 1.0(�I���_) �̊Ԃŕω�����ϐ��Ȃ̂ŁA
		// ��������ɂ��Ĉʒu���v�Z����
		var r = (dr - sr) * tm + sr;
		var s = (ds - ss) * tm + ss;
		var x = (dx - sx) * tm + sx;
		var y = (dy - sy) * tm + sy;
		
		var rc = Math.cos(r);
		var rs = Math.cos((Math.PI/2.0) - r);
		
		m00 = s * rc;
		m01 = s * -rs;
		m10 = s * rs;
		m11 = s * rc;
		mtx = (m00*-cx) + (m10*-cy) + x;
		mty = (m01*-cx) + (m11*-cy) + y;
		
		// �ړ�
		func(m00, m01, m10, m11, mtx, mty);
	}

	function continuousHandler(tick)
	{
		// �n���h��
		if(nextstop)
		{
			// �I��
			finish();
			return;
		}

		// ���Ԃ𓾂�
		var tm = tick - startTick;
		tm /= time;
		if(tm >= 1)
		{
			nextstop = true;
			tm = 1;
		}
		else
		{
			// �����v�Z
			if(accel < 0)
			{
				// �㌷ ( �ŏ��������������A���X�ɒx���Ȃ� )
				tm = 1.0 - tm;
				tm = Math.pow(tm, -accel);
				tm = 1.0 - tm;
			}
			else if(accel > 0)
			{
				// ���� ( �ŏ��͓������x���A���X�ɑ����Ȃ� )
				tm = Math.pow(tm, accel);
			}
		}

		// �ړ�
		moveFunc(moveAt, tm);
	}

	function finish()
	{
		// �ŏI�ʒu�Ɉړ�
		moveFunc(moveAt, 1.0);
		
		// ��]�Y�[���̏I��
		stop(); // ��~
	}

	function stop()
	{
		// ��~
		if(moving)
		{
			window.trigger('zoomrot');
			System.removeContinuousHandler(continuousHandler);
			moving = false;
		}
		invalidate tempLayer if tempLayer !== void;
		tempLayer = void;
		targetLayer = void;
	}

	function onStore(f, elm)
	{
		// �x��ۑ�����Ƃ�
	}

	function onRestore(f, clear, elm)
	{
		// �x��ǂݏo���Ƃ�
		stop(); // ������~
	}

	function onStableStateChanged(stable)
	{
	}

	function onMessageHiddenStateChanged(hidden)
	{
	}

	function onCopyLayer(toback)
	{
	}

	function onExchangeForeBack()
	{
	}
}

kag.addPlugin(global.zoomrot_object = new ZoomRotPlugin(kag));
	// �v���O�C���I�u�W�F�N�g���쐬���A�o�^����

@endscript
@endif

; �}�N���o�^
@macro name="zoomrot"
@eval exp="zoomrot_object.startZoomRot(mp.storage,mp.key, mp.layer,mp.page, mp.cx,mp.cy, mp.sr,mp.ss,mp.sx,mp.sy, mp.dr,mp.ds,mp.dx,mp.dy, mp.time,mp.accel)"
@endmacro

@macro name="wzoomrot"
@if exp="zoomrot_object.moving"
@waittrig * name="zoomrot" onskip="zoomrot_object.finish()"
@endif
@endmacro

@return
