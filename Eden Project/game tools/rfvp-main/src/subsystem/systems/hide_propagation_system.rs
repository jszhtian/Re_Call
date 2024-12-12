use crate::subsystem::components::{
    maths::hierarchy::{Children, Parent},
    Hide, HidePropagated,
};
use crate::subsystem::world::{GameData, World};

/// System responsible to add a `HidePropagate` component to each child of entities that have an `Hide` component
pub(crate) fn hide_propagation_system(data: &mut GameData) {
    let mut to_add = Vec::new();

    for (_, (_, children)) in data.query::<(&Hide, &Children)>().iter() {
        children.0.iter().for_each(|child| {
            let mut child_entry = data
                .entry::<&HidePropagated>(*child)
                .expect("Unreachable child during hide propagation");
            if let None = child_entry.get() {
                to_add.push(*child);
            }
        });
    }

    to_add.drain(0..).for_each(|e| {
        let _r = data.add_components(e, (HidePropagated,));
    });
}

/// System responsible to remove all the `HidePropagated` components when the parent is no longer Hidden
pub(crate) fn hide_propagated_deletion_system(data: &mut GameData) {
    let mut child_to_clear = Vec::new();
    for (e, (_c, parent)) in data.query::<(&HidePropagated, &Parent)>().iter() {
        if data
            .entry::<&Hide>(parent.0)
            .expect("Unreachable parent during hide propagated deletion")
            .get()
            .is_none()
        {
            child_to_clear.push(e);
        }
    }
    child_to_clear.drain(0..).for_each(|e| {
        let _r = data.remove_component::<HidePropagated>(e);
    })
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::{
        subsystem::components::{
            maths::hierarchy::{Children, Parent},
            Hide, HidePropagated,
        },
        subsystem::world::GameData,
    };

    #[test]
    fn hide_propagation_test() {
        let mut world = GameData::default();

        let child = world.push((2,));
        let _parent = world.push((1, Hide, Children(vec![child])));

        assert_eq!(true, world.entry::<&HidePropagated>(child).unwrap().get().is_none());

        hide_propagation_system(&mut world);

        assert_eq!(true, world.entry::<&HidePropagated>(child).unwrap().get().is_some());
    }

    #[test]
    fn hide_propagated_deletion_test() {
        let mut world = GameData::default();

        let parent = world.push((1,));
        let child = world.push((2, HidePropagated, Parent(parent)));

        assert_eq!(true, world.entry::<&HidePropagated>(child).unwrap().get().is_some());

        hide_propagated_deletion_system(&mut world);

        assert_eq!(true, world.entry::<&HidePropagated>(child).unwrap().get().is_none());
    }
}
