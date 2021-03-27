TEMPLATE = subdirs

SUBDIRS += \
  actors \
  directoryProcessorPlugin \
  fileschemaplugin \
  htmlProcessorPlugin \
  httpschemaplugin \
  indexing-client \
  indexing-service \
#  test \
  testactormanager \
  testactors \
  libs/QGumboParser


test.depends = actors
