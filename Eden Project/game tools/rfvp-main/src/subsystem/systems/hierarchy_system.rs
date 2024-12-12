
use std::collections::{HashMap, HashSet};


use log::trace;

use crate::subsystem::components::maths::hierarchy::{Children, Parent};
use crate::subsystem::world::{GameData, World};

/// System responsible to add/modify Children components to the entities referenced by a Parent component
/// If the parent component referenced by the Children one is not found, then it deletes the entity
/// If an entity has a Children component referencing non existing children, then this system will remove these references
pub(crate) fn children_manager_system(data: &mut GameData) {
    let mut parents = fetch_parent_entities(data);
    let mut component_to_add = HashMap::new();
    let mut entities_to_remove = HashSet::new();

    for (e, children) in data.query_mut::<&mut Children>() {
        if let Some(children_entity) = parents.remove(&e) {
            children.0 = children_entity;
            parents.remove(&e);
        } else{
            entities_to_remove.insert(e);
        }
    }
    parents.drain().for_each(|(parent,children)|{
        if data.contains(parent) {
            component_to_add.insert(parent, children);
        }else{
            entities_to_remove.extend(&children);
        }
    });
    component_to_add.drain().for_each(|(e, children)| {
        trace!("Adding children component on entity {:?}", e);
        let _r = data.add_components(e, (Children(children),));
    });
    entities_to_remove.drain().for_each(|e| {
        trace!("Removing entity {:?} because the parent has not been found", e);
        let _r = data.remove(e);
    });
}

fn fetch_parent_entities(data: &mut GameData) -> HashMap<hecs::Entity, Vec<hecs::Entity>> {
    let mut res = HashMap::new();
    for (e, p) in data.query::<&Parent>().iter() {
        let entry = res.entry(p.0).or_insert_with(Vec::new);
        entry.push(e);
    }
    res
}

#[cfg(test)]
mod tests {

    use super::*;
    use crate::subsystem::components::maths::hierarchy::{Children, Parent};
    use crate::subsystem::world::World;

    #[test]
    fn children_manager_system_test_children_delete() {
        let mut world = GameData::default();

        let parent = world.push((1,));
        let child = world.push((Parent(parent),));

        // First we test that the parent has no Children component
        assert_eq!(true, world.entry::<&Children>(parent).unwrap().get().is_none());

        // Then we execute the system and test that we have the good result
        children_manager_system(&mut world);

        assert_eq!(true, world.entry::<&Children>(parent).unwrap().get().is_some());

        // Finally we delete the parent entity and check that after a schedule, the child entity is also deleted
        let _r = world.remove(parent);
        children_manager_system(&mut world);

        assert_eq!(true, world.entry::<&Parent>(child).is_err());
    }

    #[test]
    fn children_manager_system_test_parent_clean() {
        let mut world = GameData::default();

        let child = world.push((1,));
        let parent = world.push((2, Children(vec![child])));

        // We check that we have the child
        assert!(
            world.entry::<&Children>(parent).unwrap().get().unwrap().0.contains(&child)
        );
        let _r = world.remove(child);

        // we delete the child and then we execute the schedule and test that we have the good result
        children_manager_system(&mut world);

        assert!(world.entry::<&Children>(parent).is_err());
    }
}
