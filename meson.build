project('npmlens', 'c',
  version: '1.0.0',
  meson_version: '>= 1.0',
  default_options: ['warning_level=2', 'c_std=gnu11'],
)

app_id = 'io.github.eljijuna.NpmLens'

dependency('gtk4')
dependency('libadwaita-1')
dependency('webkitgtk-6.0')

subdir('data')
subdir('src')
