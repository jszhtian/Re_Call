use hecs::{Component, Entity};
use std::{cfg, collections::HashMap, ops::Range, path::Path, time::SystemTime};

use wgpu::{util::DeviceExt, BindGroup, BindGroupLayout, Buffer, CommandEncoder, Device, Queue, RenderPassColorAttachment, RenderPipeline, SurfaceConfiguration, TextureView, SamplerBindingType, TextureFormat, StoreOp};

use crate::subsystem::world::{GameData, World};
use crate::{
    config::app_config::AppConfig,
    subsystem::components::{
        color::Color,
        material::{Material, Texture},
        maths::{camera::Camera, transform::Transform},
        tiles::sprite::Sprite,
        ui::{ui_image::UiImage, ui_text::UiTextImage, UiComponent},
        Hide, HidePropagated,
    },
    rendering::{
        gl_representations::{GlUniform, UniformData},
        shaders::pipeline::pipeline,
        Renderable2D, RenderableUi, Renderer,
    },
    utils::file::{read_file_modification_time, FileReaderError},
};

#[derive(Default)]
pub(crate) struct Shinku2D {
    vertex_buffers: HashMap<Entity, Buffer>,
    index_buffers: HashMap<Entity, Buffer>,
    render_pipelines: HashMap<String, RenderPipeline>,
    texture_bind_group_layout: Option<BindGroupLayout>,
    transform_bind_group_layout: Option<BindGroupLayout>,
    diffuse_bind_groups: HashMap<String, (BindGroup, wgpu::Texture)>,
    transform_uniform_bind_groups: HashMap<Entity, (GlUniform, Buffer, BindGroup)>,
    assets_timestamps: HashMap<String, SystemTime>,
}

struct RenderingInfos {
    layer: usize,
    range: Range<u32>,
    entity: Entity,
    texture_path: Option<String>,
    type_name: String,
}

impl Renderer for Shinku2D {
    fn start(&mut self, device: &Device, surface_config: &SurfaceConfiguration) {
        let uniform_bind_group_layout =
            device.create_bind_group_layout(&wgpu::BindGroupLayoutDescriptor {
                entries: &[wgpu::BindGroupLayoutEntry {
                    binding: 0,
                    visibility: wgpu::ShaderStages::VERTEX,
                    ty: wgpu::BindingType::Buffer {
                        ty: wgpu::BufferBindingType::Uniform,
                        has_dynamic_offset: false,
                        min_binding_size: None,
                    },
                    count: None,
                }],
                label: Some("uniform_bind_group_layout"),
            });

        let texture_bind_group_layout =
            device.create_bind_group_layout(&wgpu::BindGroupLayoutDescriptor {
                entries: &[
                    wgpu::BindGroupLayoutEntry {
                        binding: 0,
                        visibility: wgpu::ShaderStages::FRAGMENT,
                        ty: wgpu::BindingType::Texture {
                            multisampled: false,
                            view_dimension: wgpu::TextureViewDimension::D2,
                            sample_type: wgpu::TextureSampleType::Float { filterable: true },
                        },
                        count: None,
                    },
                    wgpu::BindGroupLayoutEntry {
                        binding: 1,
                        visibility: wgpu::ShaderStages::FRAGMENT,
                        ty: wgpu::BindingType::Sampler(SamplerBindingType::Filtering),
                        count: None,
                    },
                ],
                label: Some("texture_bind_group_layout"),
            });

        self.transform_bind_group_layout = Some(uniform_bind_group_layout);
        self.texture_bind_group_layout = Some(texture_bind_group_layout);
        self.insert_components_pipelines::<Sprite>(&device, &surface_config);
        self.insert_components_pipelines::<UiImage>(&device, &surface_config);
        self.insert_components_pipelines::<UiTextImage>(&device, &surface_config);
    }

    fn update(
        &mut self,
        data: &mut GameData,
        device: &Device,
        surface_config: &SurfaceConfiguration,
        queue: &mut Queue,
    ) {
        if world_contains_camera(data) {
            self.update_diffuse_bind_groups(data, device, queue);
            self.update_transforms(data, &device, queue);
            self.upsert_component_buffers::<Sprite>(data, &device);
            self.upsert_ui_component_buffers::<UiImage>(data, &device, &surface_config, queue);
            self.upsert_ui_component_buffers::<UiTextImage>(data, &device, &surface_config, queue);
        } else {
            log::warn!("No camera has been found in resources");
        }
        self.clean_buffers(data);
    }

    fn render(
        &mut self,
        data: &mut GameData,
        config: &AppConfig,
        texture_view: &TextureView,
        encoder: &mut CommandEncoder,
    ) {
        if world_contains_camera(data) {
            let mut rendering_infos = Vec::new();
            rendering_infos.append(&mut self.pre_render_component::<Sprite>(data));
            rendering_infos.append(&mut self.pre_render_ui_component::<UiImage>(data));
            rendering_infos.append(&mut self.pre_render_ui_component::<UiTextImage>(data));

            rendering_infos.sort_by(|a, b| b.layer.cmp(&a.layer));

            self.render_component(config, texture_view, encoder, rendering_infos);
        }
    }
}

impl Shinku2D {
    fn insert_components_pipelines<T: Component + Renderable2D>(
        &mut self,
        device: &&Device,
        surface_config: &&SurfaceConfiguration,
    ) {
        self.insert_pipeline_if_not_finded::<T>(device, surface_config);
    }

    fn upsert_component_buffers<T: Component + Renderable2D>(
        &mut self,
        data: &mut GameData,
        device: &&Device,
    ) {
        for (entity, (component, material, _)) in
            data.query_mut::<(&mut T, &Material, &Transform)>()
        {
            if !self.vertex_buffers.contains_key(&entity) || component.dirty() {
                let vertex_buffer =
                    device.create_buffer_init(&component.vertex_buffer_descriptor(Some(material)));
                self.vertex_buffers.insert(entity, vertex_buffer);
            }

            if !self.index_buffers.contains_key(&entity) || component.dirty() {
                let index_buffer =
                    device.create_buffer_init(&component.indexes_buffer_descriptor());
                self.index_buffers.insert(entity, index_buffer);
            }

            component.set_dirty(false);
        }
    }

    fn upsert_ui_component_buffers<T: Component + Renderable2D + RenderableUi>(
        &mut self,
        data: &mut GameData,
        device: &&Device,
        _surface_config: &&SurfaceConfiguration,
        _queue: &mut Queue,
    ) {
        for (entity, (component, _, m)) in data.query::<(&mut T, &Transform, Option<&Material>)>().iter() {
            self.vertex_buffers.entry(entity).or_insert_with(|| {
                let vertex_buffer =
                    device.create_buffer_init(&component.vertex_buffer_descriptor(m));
                vertex_buffer
            });

            self.index_buffers.entry(entity).or_insert_with(|| {
                let index_buffer =
                    device.create_buffer_init(&component.indexes_buffer_descriptor());
                index_buffer
            });
        }
    }

    fn insert_pipeline_if_not_finded<T: Component + Renderable2D>(
        &mut self,
        device: &&Device,
        surface_config: &&SurfaceConfiguration,
    ) {
        let type_name = std::any::type_name::<T>();
        if !self.render_pipelines.contains_key(type_name) {
            self.render_pipelines.insert(
                type_name.to_string(),
                pipeline(
                    device,
                    surface_config,
                    self.texture_bind_group_layout.as_ref().unwrap(),
                    self.transform_bind_group_layout.as_ref().unwrap(),
                    T::topology(),
                ),
            );
        }
    }

    fn render_component(
        &mut self,
        config: &AppConfig,
        texture_view: &TextureView,
        encoder: &mut CommandEncoder,
        mut infos: Vec<RenderingInfos>,
    ) {
        let mut render_pass = encoder.begin_render_pass(&wgpu::RenderPassDescriptor {
            label: None,
            color_attachments: &[get_default_color_attachment(texture_view, config)],
            depth_stencil_attachment: None,
            occlusion_query_set: None,
            timestamp_writes: None
        });

        while let Some(rendering_infos) = infos.pop() {
            render_pass.set_bind_group(
                1,
                &self.transform_uniform_bind_groups.get(&rendering_infos.entity).unwrap().2,
                &[],
            );
            render_pass.set_vertex_buffer(
                0,
                self.vertex_buffers.get(&rendering_infos.entity).as_ref().unwrap().slice(..),
            );
            render_pass.set_index_buffer(
                self.index_buffers.get(&rendering_infos.entity).as_ref().unwrap().slice(..),
                wgpu::IndexFormat::Uint16,
            );
            render_pass.set_pipeline(
                self.render_pipelines.get(rendering_infos.type_name.as_str()).as_ref().unwrap(),
            );
            if let Some(path) = rendering_infos.texture_path {
                render_pass.set_bind_group(
                    0,
                    &self.diffuse_bind_groups.get(path.as_str()).unwrap().0,
                    &[],
                );
            }

            render_pass.draw_indexed(rendering_infos.range, 0, 0..1);
        }
    }

    fn pre_render_component<T: Component + Renderable2D>(
        &mut self,
        data: &mut GameData,
    ) -> Vec<RenderingInfos> {
        let type_name = std::any::type_name::<T>();
        let mut render_infos = Vec::new();
        for (entity, (component, material, transform)) in data
            .query::<(&mut T, &Material, &Transform)>()
            .without::<&Hide>()
            .without::<&HidePropagated>()
            .iter()
        {
            let path = match material {
                Material::Color(color) => Some(get_path_from_color(&color)),
                Material::Texture(p) => Some(p.clone()),
            };
            render_infos.push(RenderingInfos {
                layer: transform.translation().z(),
                range: component.range(),
                entity,
                texture_path: path,
                type_name: type_name.to_string(),
            });
        }
        render_infos
    }

    fn pre_render_ui_component<T: Component + Renderable2D + RenderableUi>(
        &mut self,
        data: &mut GameData,
    ) -> Vec<RenderingInfos> {
        let type_name = std::any::type_name::<T>();
        let mut render_infos = Vec::new();
        for (entity, (component, transform, material)) in
            data.query::<(&mut T, &Transform, Option<&Material>)>()
                .without::<&Hide>()
                .without::<&HidePropagated>()
                .iter()
        {
            let path = if material.is_some() {
                match material.unwrap() {
                    Material::Color(color) => Some(get_path_from_color(&color)),
                    Material::Texture(p) => Some(p.clone()),
                    _ => None
                }
            }else{
                None
            };

            render_infos.push(RenderingInfos {
                layer: transform.translation().z(),
                range: component.range(),
                entity,
                texture_path: path,
                type_name: type_name.to_string(),
            });
        }
        render_infos
    }

    fn update_transforms(&mut self, data: &mut GameData, device: &&Device, queue: &mut Queue) {
        self.update_transforms_for_type::<Sprite>(data, &device, queue);
        self.update_transforms_for_type::<UiImage>(data, &device, queue);
        self.update_transforms_for_type::<UiTextImage>(data, &device, queue);
    }

    fn update_transforms_for_type<T: Component + Renderable2D>(
        &mut self,
        data: &mut GameData,
        device: &&Device,
        queue: &mut Queue,
    ) {
        let camera1 = {
            let mut t = Transform::default();
            let mut c = Camera::new(1.0, 1.0);

            for (_, (cam, tra)) in data.query::<(&Camera, &Transform)>().iter() {
                c = cam.clone();
                t = tra.clone();
            }
            (c, t)
        };

        let camera = (&camera1.0, &camera1.1);

        for (entity, (transform, optional_ui_component, _)) in
            data.query::<(&Transform, Option<&UiComponent>, &T)>().iter()
        {
            if let std::collections::hash_map::Entry::Vacant(e) = self.transform_uniform_bind_groups.entry(entity) {
                let (uniform, uniform_buffer, group) = create_transform_uniform_bind_group(
                    &device,
                    transform,
                    camera,
                    optional_ui_component.is_some(),
                    self.transform_bind_group_layout.as_ref().unwrap(),
                );
                queue.write_buffer(&uniform_buffer, 0, bytemuck::cast_slice(&[uniform]));
                e.insert((uniform, uniform_buffer, group));
            } else {
                let (uniform, uniform_buffer, _) = self
                    .transform_uniform_bind_groups
                    .get_mut(&entity)
                    .expect("Fatal error, a transform has been marked as found but doesn't exist");
                uniform.replace_with(GlUniform::from(UniformData {
                    transform,
                    camera,
                    is_ui_component: optional_ui_component.is_some(),
                }));
                queue.write_buffer(uniform_buffer, 0, bytemuck::cast_slice(&[*uniform]));
            }
        }
    }

    fn texture_should_be_reloaded(
        &self,
        path: &str,
        new_timestamp: &Option<Result<SystemTime, FileReaderError>>,
    ) -> bool {
        !self.diffuse_bind_groups.contains_key(path)
            || if let Some(Ok(timestamp)) = new_timestamp {
                !self.assets_timestamps.contains_key(path)
                    || !self.assets_timestamps.get(path).unwrap().eq(timestamp)
            } else {
                false
            }
    }

    /// Loads in the queue materials that are not yet loaded.
    fn update_diffuse_bind_groups(
        &mut self,
        data: &mut GameData,
        device: &Device,
        queue: &mut Queue,
    ) {
        let hot_timer_cycle = if cfg!(feature = "hot-reload") {
            let mut timers = data.timers();
            let hot_reload_timer =
                timers.get_timer("hot-reload-timer").expect("Missing mandatory timer : hot_reload");
            hot_reload_timer.cycle() > 0
        } else {
            false
        };

        for (_entity, material) in data.query::<&Material>().iter() {
            match material {
                Material::Texture(texture_path) => {
                    let path = Path::new(texture_path.as_str());
                    let new_timestamp = if hot_timer_cycle
                        || !self.diffuse_bind_groups.contains_key(texture_path.as_str())
                    {
                        Some(read_file_modification_time(path))
                    } else {
                        None
                    };

                    if self.texture_should_be_reloaded(&texture_path, &new_timestamp) {
                        if self.diffuse_bind_groups.contains_key(texture_path.as_str()) {
                            self.diffuse_bind_groups
                                .get(texture_path.as_str())
                                .expect("Unreachable diffuse bind group after check")
                                .1
                                .destroy();
                            self.diffuse_bind_groups.remove(texture_path.as_str());
                        }

                        // Check if this is an in_memory_texture from font_atlas
                        let loaded_texture = match data.font_atlas().get_texture_from_path(texture_path){
                            Some(t) => t.take_texture(),
                            None => Texture::from_png(path)
                        };

                        self.diffuse_bind_groups.insert(
                            texture_path.clone(),
                            load_texture_to_queue(
                                &loaded_texture,
                                queue,
                                device,
                                self.texture_bind_group_layout.as_ref().unwrap(),
                            ),
                        );

                        if let Some(Ok(timestamp)) = new_timestamp {
                            self.assets_timestamps.insert(texture_path.clone(), timestamp);
                        }
                    }
                }
                Material::Color(color) => {
                    let path = get_path_from_color(&color);
                    if !self.diffuse_bind_groups.contains_key(path.as_str()) {
                        let loaded_texture = Texture::from_color(&color);
                        self.diffuse_bind_groups.insert(
                            path.clone(),
                            load_texture_to_queue(
                                &loaded_texture,
                                queue,
                                device,
                                self.texture_bind_group_layout.as_ref().unwrap(),
                            ),
                        );
                    }
                }
            }
        }
    }

    fn clean_buffers(&mut self, data: &mut GameData) {
        self.vertex_buffers.retain(|&k, _| data.contains(k));
        self.index_buffers.retain(|&k, _| data.contains(k));
        self.transform_uniform_bind_groups.retain(|&k, _| data.contains(k));
    }
}

fn load_texture_to_queue(
    texture: &Texture,
    queue: &mut Queue,
    device: &Device,
    texture_bind_group_layout: &BindGroupLayout,
) -> (BindGroup, wgpu::Texture) {
    let texture_size = wgpu::Extent3d {
        width: texture.width as u32,
        height: texture.height as u32,
        depth_or_array_layers: 1,
    };

    let diffuse_texture = device.create_texture(&wgpu::TextureDescriptor {
        size: texture_size,
        mip_level_count: 1,
        sample_count: 1,
        dimension: wgpu::TextureDimension::D2,
        usage: wgpu::TextureUsages::TEXTURE_BINDING | wgpu::TextureUsages::COPY_DST,
        label: Some("diffuse_texture"),
        format: TextureFormat::Rgba8UnormSrgb,
        view_formats: &[TextureFormat::Rgba8UnormSrgb]
    });

    queue.write_texture(
        wgpu::ImageCopyTexture {
            texture: &diffuse_texture,
            mip_level: 0,
            origin: wgpu::Origin3d::ZERO,
            aspect: wgpu::TextureAspect::All,
        },
        &*texture.bytes,
        wgpu::ImageDataLayout {
            offset: 0,
            bytes_per_row: Some(4 * &texture.width),
            rows_per_image: Some(1 * &texture.height),
        },
        texture_size,
    );
    let diffuse_texture_view = diffuse_texture.create_view(&wgpu::TextureViewDescriptor::default());
    let diffuse_sampler = device.create_sampler(&wgpu::SamplerDescriptor {
        address_mode_u: wgpu::AddressMode::ClampToEdge,
        address_mode_v: wgpu::AddressMode::ClampToEdge,
        address_mode_w: wgpu::AddressMode::ClampToEdge,
        mag_filter: wgpu::FilterMode::Nearest,
        min_filter: wgpu::FilterMode::Nearest,
        mipmap_filter: wgpu::FilterMode::Nearest,
        ..Default::default()
    });

    let diffuse_bind_group = device.create_bind_group(&wgpu::BindGroupDescriptor {
        layout: &texture_bind_group_layout,
        entries: &[
            wgpu::BindGroupEntry {
                binding: 0,
                resource: wgpu::BindingResource::TextureView(&diffuse_texture_view),
            },
            wgpu::BindGroupEntry {
                binding: 1,
                resource: wgpu::BindingResource::Sampler(&diffuse_sampler),
            },
        ],
        label: Some("diffuse_bind_group"),
    });

    (diffuse_bind_group, diffuse_texture)
}

fn create_transform_uniform_bind_group(
    device: &Device,
    transform: &Transform,
    camera: (&Camera, &Transform),
    is_ui_component: bool,
    uniform_bind_group_layout: &BindGroupLayout,
) -> (GlUniform, Buffer, BindGroup) {
    let uniform = GlUniform::from(UniformData { transform, camera, is_ui_component });
    let uniform_buffer = device.create_buffer_init(&wgpu::util::BufferInitDescriptor {
        label: Some("Uniform Buffer"),
        contents: bytemuck::cast_slice(&[uniform]),
        usage: wgpu::BufferUsages::UNIFORM | wgpu::BufferUsages::COPY_DST,
    });

    let uniform_bind_group = device.create_bind_group(&wgpu::BindGroupDescriptor {
        layout: uniform_bind_group_layout,
        entries: &[wgpu::BindGroupEntry {
            binding: 0,
            resource: uniform_buffer.as_entire_binding(),
        }],
        label: Some("uniform_bind_group"),
    });

    (uniform, uniform_buffer, uniform_bind_group)
}

fn get_default_color_attachment<'a>(
    texture_view: &'a TextureView,
    config: &'a AppConfig,
) -> Option<RenderPassColorAttachment<'a>> {
    Some(RenderPassColorAttachment {
        view: texture_view,
        resolve_target: None,
        ops: wgpu::Operations {
            load: wgpu::LoadOp::Clear(
                if let Some(color) = &config
                    .window_config
                    .as_ref()
                    .expect("Window config is missing")
                    .default_background_color
                {
                    wgpu::Color {
                        r: (color.red() as f32 / 255.) as f64,
                        g: (color.green() as f32 / 255.) as f64,
                        b: (color.blue() as f32 / 255.) as f64,
                        a: color.alpha() as f64,
                    }
                } else {
                    wgpu::Color { r: 1., g: 0., b: 0., a: 1.0 }
                },
            ),
            store: StoreOp::Store,
        },
    })
}

fn get_path_from_color(color: &Color) -> String {
    format!("color-{}-{}-{}-{}", color.red(), color.green(), color.blue(), color.alpha())
}

fn world_contains_camera(data: &mut GameData) -> bool {
    data.query::<&Camera>().iter().count() > 0
}
