@if exp="typeof(global.world_object) == 'undefined'"
@iscript

KAGLoadScript('world.tjs');

kag.addPlugin(global.world_object = new KAGWorldPlugin(kag));
if (kag.debugLevel >= tkdlSimple) {
    dm("���[���h���ݒ芮��");
}

@endscript
@endif

@return
