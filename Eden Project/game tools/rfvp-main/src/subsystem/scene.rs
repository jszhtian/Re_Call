use crate::subsystem::world::GameData;

/// Trait to implement in order to define a `Scene`.
pub trait Scene {
    /// Will be called once before the new game loop iteration. Useful to initialize resources and add everything you need in the world.
    fn on_start(&mut self, _data: &mut GameData) {}
    /// Will be called each game loop, before the systems execution
    fn on_update(&mut self, _data: &mut GameData) {}
    /// Will be called each game loop, after the systems execution
    fn late_update(&mut self, _data: &mut GameData) {}
    /// Will be called for deleted scene at the end of the frame where it was deleted
    fn on_stop(&mut self, _data: &mut GameData) {}
}

pub(crate) enum SceneAction {
    Update,
    Start,
    EndFrame,
    LateUpdate,
}

/// `SceneMachine` is the Resource used to control the game scene.
#[derive(Default)]
pub(crate) struct SceneMachine {
    pub(crate) current_scene: Option<Box<dyn Scene>>,
}

impl SceneMachine {
    pub(crate) fn apply_scene_action(&mut self, action: SceneAction, data: &mut GameData) {
        if let Some(scene) = self.current_scene.as_mut() {
            match action {
                SceneAction::Update => scene.on_update(data),
                SceneAction::Start => scene.on_start(data),
                SceneAction::EndFrame => {}
                SceneAction::LateUpdate => scene.late_update(data),
            };
        }

        #[allow(clippy::single_match)]
        match action {
            SceneAction::EndFrame => {
                let action = data.scene_controller().action();
                match action {
                    Some(SceneTrans::Switch(mut new_scene)) => {
                        if let Some(mut scene) = self.current_scene.take() {
                            scene.on_stop(data);
                        }
                        new_scene.on_start(data);
                        self.current_scene = Some(new_scene);
                    }
                    _ => {}
                }
            }
            _ => {}
        }
    }
}

pub(crate) enum SceneTrans {
    Switch(Box<dyn Scene>),
}

/// `SceneController` is the Resource used to control the game scenes.
#[derive(Default)]
pub struct SceneController {
    /// scene action that has to be executed at the end of the frame
    pub(crate) action: Option<SceneTrans>,
}

impl SceneController {
    /// Replace the current scene with the name `` with the scene created from type `T`. (Useful for level switching).
    /// Note that the scene's stop will happen at the end of the frame.
    pub fn switch<T: Scene + Default + 'static>(&mut self) {
        self.action = Some(SceneTrans::Switch(Box::<T>::default()));
    }

    pub(crate) fn action(&mut self) -> Option<SceneTrans> {
        self.action.take()
    }
}

