@if exp="typeof(global.alphamovie_object) == 'undefined'"
@iscript
const numAlphaMovies = 3;

Plugins.link("AlphaMovie.dll");

// �������
class AlphaMovieLinearFrameMover
{
	var alphamov;			// ������
	var path;				// �t���[���z�� -> �t���[���ԍ�, opacity �������Ă���
	var fps;				// �t���[�����[�g
	var pointCount;			// path.count -1
	var finalFunction;		// �ړ��I�����ɌĂԊ֐�
	var startTick;			// �ړ��J�n�������e�B�b�N
	var totalTime;			// �S�̂̎���
	var moving = false;		// �ړ�����
	var first = true;		// ����̃n���h���Ăяo����
	var nextStop = false;	// ���̃n���h���Ăяo���Œ�~���邩

	var curPoint;

	function AlphaMovieLinearFrameMover(alphamov, path, fps, finalfunction ) {
		this.alphamov = alphamov;
		this.path = path;
		this.fps = fps;
		this.finalFunction = finalfunction;
		pointCount = (path.count \ 2 - 1);
		totalTime = (path[int(pointCount*2)] * 1000) \ fps;
	}

	function finalize() {
		stopMove();
	}

	function startMove() {
		first = true;
		System.addContinuousHandler(handler); // ContinuousHandlers �ɒǉ�
		moving = true;
	}

	function stopMove() {
		// �ړ����I���
		if( moving ) {
			// �ŏI�ʒu�Ɉړ�
			if(alphamov isvalid) {
				var idx = int(pointCount*2);
				var p = path;
				alphamov.opacity = p[idx+1];
			}
			System.removeContinuousHandler(handler);
			moving = false;
			finalFunction();
		}
	}

	function handler(tick) {
		// �ړ����ɌĂ΂��
		if(nextStop || !(alphamov isvalid)) { stopMove();  return; }
		if(first) startTick = tick, first = false, curPoint = 0;
		tick -= startTick;
		if( tick >= totalTime ) {
			nextStop = true;
			var idx = int(pointCount*2);
			var p = path;
			alphamov.opacity = p[idx+1];
			return;
		}
		move(tick);
	}

	function move(tick) {
		var idx = int(curPoint*2);
		var p = path;
		var curTime = (p[idx] * 1000) \ fps;
		var nextIdx = idx + 2;
		var nextTime = (p[nextIdx] * 1000) \ fps;
		if( tick > nextTime ) {
			curPoint++;
			idx = int(curPoint*2);
			curTime = (p[idx] * 1000) \ fps;
			nextIdx = idx + 2;
			nextTime = (p[nextIdx] * 1000) \ fps;
		}

		var duration = nextTime - curTime;
		var diff = (tick - curTime);

		var ratio = diff / duration;
		var p = path;
		var so = p[idx+1];
		var eo = p[nextIdx+1];
		var o = eo >= 256 ? so : int((eo-so)*ratio + so);
		alphamov.opacity = o;
	}
}

class AlphaMoviePlayer extends AlphaMovie
{
	var		moving = false; // �^�C�}�[���n�����Ă��邩�ǂ���
	var		lastTick;
	var		drawlayer;
	var		drawlayerBack;
	var		alphaMovie;
	var		lastFrame;
	var		remain;
	var		movieVisible;
	var		slot;
	var		hasNext;
	var		currentNumberOfFrame;
	var		currentLoop;

	var		moveObject; // ���ݐi�s���̎����ړ��p�I�u�W�F�N�g(�i�s���Ă��Ȃ��Ƃ���void)

	function AlphaMoviePlayer( index ) {
		this.moving = false;
		this.drawlayer = new CharacterLayer( kag, kag.fore.base, "alpha movie 00" + index, 1234 + index );
		CreateDrawLayer( kag.fore.base, this.drawlayer, index );

		this.drawlayerBack = new CharacterLayer( kag, kag.back.base, "alpha movie 00" + index, 1234 + index );
		CreateDrawLayer( kag.back.base, this.drawlayerBack, index );
		this.movieVisible = true;
		this.slot = index;
		this.hasNext = false;
		this.isLoopNext = false;
		this.currentNumberOfFrame = 0;
	}

	function finalize() {
		invalidate moveObject if moveObject !== void;
		stop();
	}

	function play() {
		if( moving ) { stop(); }

		super.play();
		if(moving == false) {
			hasNext = false;
			remain = 0;
			currentNumberOfFrame = numOfFrame;
			currentLoop = loop;
			lastTick = System.getTickCount();
			System.addContinuousHandler(handler);
			moving = true;
		}
	}
	function CreateDrawLayer( parent, target, index ) {
		target.type = ltAddAlpha;
		target.face = dfAddAlpha;
		target.absolute = 100000;
		target.visible = false;
	}

	function stop() {
		if(moving) {
			System.removeContinuousHandler(handler);
			moving = false;
		}
		super.stop();
	}

	function setNextMovieFile( storage ) {
		super.setNextMovieFile( storage );
		hasNext = true;
	}

//	interval > scale * 1000 / Rate
//	interval * rate / scale > 1000

	function handler(tick) {
		var		interval = tick - lastTick;
		var		rate = (int)( interval * FPSRate / FPSScale + remain );
		if( rate > 1000 ) {
			var ret = showNextImage( drawlayer );
			if( drawlayer.visible == false && movieVisible == true ) {
				drawlayer.visible = true;
			}
			if( drawlayerBack.visible == true ) {
				drawlayerBack.visible = false;
			}
//			if( loop == false ) {
//				if( ret == (numOfFrame-1) ) {
			if( currentLoop == false ) {
				if( ret == (currentNumberOfFrame-1) ) {
//					dm("next?" + hasNext);
					if( hasNext == false ) {
						stop();
						kag.trigger( "finish_alpha_movie_" + slot );
					} else {
						hasNext = false;
						currentNumberOfFrame = numOfFrame;
						currentLoop = loop;
						kag.trigger( "next_alpha_movie_" + slot );
					}
				}
			}
			lastTick = tick;
			remain = rate - 1000;
		}
	}
	function swapFronBack() {
		var		tmp;
		tmp = drawlayerBack;
		drawlayerBack = drawlayer;
		drawlayer = tmp;
	}
	function swapFronBackVisible() {
		var		tmp;
		tmp = drawlayerBack.visible;
		drawlayerBack.visible = drawlayer.visible;
		drawlayer.visible = tmp;
	}
	function moveFinalFunction() {
		// �����ړ����I������Ƃ��ɌĂ΂��֐�
		drawlayer.window.moveCount--;
		drawlayer.window.onLayerMoveStop();
	}

	function beginFrameMove(elm) {
		// elm �ɏ]�������ړ����J�n����
		stopMove();

		var fps = elm.fps === void ? 30 : +elm.fps;

		// path �̕���
		var array = [].split("(), ", elm.path, , true);
		for(var i = array.count-1; i>=0; i--) array[i+2] = +array[i];
		array[0] = 0;
		array[1] = opacity;

		// �ړ��p�I�u�W�F�N�g�̍쐬
		moveObject = new AlphaMovieLinearFrameMover(this, array, fps, moveFinalFunction );

		drawlayer.window.moveCount++;
		moveObject.startMove();
	}

	function stopMove() {
		if(moveObject !== void) invalidate moveObject, moveObject = void;
	}

	property absolute {
		getter { return drawlayer.absolute; }
		setter(x) { drawlayer.absolute = x; drawlayerBack.absolute = x; }
	}
	property type {
		getter { return drawlayer.type; }
		setter(x) { drawlayer.type = x; drawlayerBack.type = x; }
	}
	property face {
		getter { return drawlayer.face; }
		setter(x) { drawlayer.face = x; drawlayerBack.face = x; }
	}
	property visible {
		getter { return movieVisible; }
		setter(x) {
			movieVisible = x;
			if( movieVisible == false ) {
				drawlayer.visible = false;
				drawlayerBack.visible = false;
			}
		}
	}
	property width { getter { screenWidth; } }
	property height { getter { screenHeight; } }
	property loop {
		getter { return super.loop; }
		setter(x) { super.loop = x; currentLoop = x; }
	}
	property opacity {
		getter { return drawlayer.opacity; }
		setter(x) { drawlayer.opacity = x; drawlayerBack.opacity = x; }
	}
}



// ������v���O�C��
class AlphaMoviePlugin extends KAGPlugin
{
	var currentStorage;
	var currentLayout;
	var movies;

	function AlphaMoviePlugin() {
		super.KAGPlugin();

		movies = new Array();
		var absolute = 100000;
		for( var i = 0; i < numAlphaMovies; i++ ) {
			movies[i] = new AlphaMoviePlayer(i);
			movies[i].absolute = absolute;
			absolute += 1000;
		}
	}

	function finalize() {
		for( var i = 0; i < movies.count; i++ ) {
			invalidate movies[i];
		}
		super.finalize(...);
	}

	function play( slot, storage ) {
		try {
			movies[slot].open( storage );
			movies[slot].play();
		} catch(e) {
			dm( e.message );
			return false;
		}
		return true;
	}

	function playmovie( elm ) {
		if( elm.storage !== void ) {
			var slot = 0;
			if( elm.slot !== void ) slot = +elm.slot;

			if( elm.left !== void ) movies[slot].left = +elm.left;
			if( elm.top !== void ) movies[slot].top = +elm.top;

			var loop = false;
			if( elm.loop !== void ) loop = +elm.loop;
			movies[slot].loop = loop;

			play( slot, elm.storage );
		}
	}
	function stopmovie( elm ) {
		var slot = 0;
		if( elm.slot !== void ) slot = +elm.slot;
		movies[slot].stop();
	}

	function waitstop( elm ) {
		var slot = 0;
		if( elm.slot !== void ) slot = +elm.slot;
		elm["name"] = "finish_alpha_movie_" + slot;
		kag.waitTrigger( elm );
	}

	function nextmovie( elm ) {
		var slot = 0;
		if( elm.slot !== void ) slot = +elm.slot;
		var nloop = false;
		if( elm.loop !== void ) nloop = +elm.loop;
		movies[slot].nextLoop = nloop;
		movies[slot].setNextMovieFile( elm.storage );
	}

	function setoption( elm ) {
		var slot = 0;
		if( elm.slot !== void ) slot = +elm.slot;

		if( elm.left !== void ) movies[slot].left = +elm.left;
		if( elm.top !== void ) movies[slot].top = +elm.top;
		if( elm.loop !== void ) movies[slot].loop = +elm.loop;
		if( elm.visible !== void ) movies[slot].visible = +elm.visible;
		if( elm.frame !== void ) movies[slot].frame = +elm.frame;
		if( elm.opacity !== void ) movies[slot].opacity = +elm.opacity;
	}
	function onCopyLayer(toback) {
		// ���C���̕\�������̏��̃R�s�[
		// backlay �^�O��g�����W�V�����̏I�����ɌĂ΂��
		// �����Ń��C���Ɋւ��ăR�s�[���ׂ��Ȃ̂�
		// �\��/��\���̏�񂾂�

		for( var i = 0; i < numAlphaMovies; i++ ) {
			movies[i].swapFronBackVisible();
		}
	}
	function onExchangeForeBack() {
		// ���ƕ\�̊Ǘ���������
		// children = true �̃g�����W�V�����ł́A�g�����W�V�����I������
		// �\��ʂƗ���ʂ̃��C���\���������������ւ��̂ŁA
		// ����܂� �\��ʂ��Ǝv���Ă������̂�����ʂɁA����ʂ��Ǝv����
		// �������̂��\��ʂɂȂ��Ă��܂��B�����̃^�C�~���O�ł��̏���
		// ����ւ���΁A�����͐����Ȃ��ōςށB

		for( var i = 0; i < numAlphaMovies; i++ ) {
			movies[i].swapFronBack();
		}
	}
	function hasNextMovie( elm ) {
		var slot = 0;
		if( elm.slot !== void ) slot = +elm.slot;
		return movies[slot].hasNext;
	}
	function isPlayingMovie( elm ) {
		var slot = 0;
		if( elm.slot !== void ) slot = +elm.slot;
		return movies[slot].moving;
	}
	function frameMove( elm ) {
		var slot = 0;
		if( elm.slot !== void ) slot = +elm.slot;
		movies[slot].beginFrameMove(elm);
		return 0;
	}
}

// �v���O�C���I�u�W�F�N�g���쐬���A�o�^����
kag.addPlugin(global.alphamovie_object = new AlphaMoviePlugin());

@endscript
@endif

; �}�N����`
; storage, slot, left, top, loop
@macro name=playamov
@eval exp="alphamovie_object.playmovie( mp )"
@endmacro

; slot
@macro name=stopamov
@eval exp="alphamovie_object.stopmovie( mp )"
@endmacro

; storage, slot, loop
@macro name=nextamov
@eval exp="alphamovie_object.nextmovie( mp )"
@endmacro

; slot, left, top, loop, visible, frame opacity
@macro name=amovopt
@eval exp="alphamovie_object.setoption( mp )"
@endmacro

; slot canskip
@macro name=wam
@if exp="alphamovie_object.isPlayingMovie( mp )"
@if exp="mp.slot !== void"
@eval exp="tf.amov_trig_name = 'finish_alpha_movie_' + mp.slot"
@waittrig name="&tf.amov_trig_name" canskip=%canskip|true
@else
@waittrig name="finish_alpha_movie_0" canskip=%canskip|true
@endif
@endif
@endmacro

; slot canskip
@macro name=wamnext
@if exp="alphamovie_object.hasNextMovie( mp )"
@if exp="mp.slot !== void"
@eval exp="tf.amov_next_trig_name = 'next_alpha_movie_' + mp.slot"
@waittrig name="&tf.amov_next_trig_name" canskip=%canskip|true
@else
@waittrig name="next_alpha_movie_0" canskip=%canskip|true
@endif
@endif
@endmacro

; slot fps path
@macro name=amovmove
@eval exp="alphamovie_object.frameMove( mp )"
@endmacro

@return

