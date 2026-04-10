gnome = import('gnome')

app_resources = gnome.compile_resources(
  'io.github.eljijuna.NpmLens',
  'io.github.eljijuna.NpmLens.gresource.xml',
  source_dir: '.',
  c_name: 'app_resources',
)

install_data(
  'io.github.eljijuna.NpmLens.desktop',
  install_dir: get_option('datadir') / 'applications',
)

install_data(
  'io.github.eljijuna.NpmLens.metainfo.xml',
  install_dir: get_option('datadir') / 'metainfo',
)

install_data(
  'icons/io.github.eljijuna.NpmLens.svg',
  install_dir: get_option('datadir') / 'icons/hicolor/scalable/apps',
)
