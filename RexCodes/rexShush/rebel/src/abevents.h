/* Event header for application - AppBuilder 2.03  */

#if defined(__cplusplus)
extern "C" {
#endif

static const ApEventLink_t AbApplLinks[] = {
	{ 3, 0, 0L, 0L, 0L, &rebelBaseWindow, NULL, NULL, 0, initGui, 2, 0, 0, 0, },
	{ 0 }
	};

ApEventLink_t AbInternalLinks[] = {
	{ 3, 0, 0L, 0L, 0L, &rebelWindow, NULL, NULL, 0, rebelWin::init, 2, 11, 0, 0, },
	{ 11, 0, 0L, 0L, 0L, &rebelPictures, NULL, NULL, 0, NULL, 0, 0, 0, 0, },
	{ 4, 0, 0L, 0L, 0L, &rebelResourceDialog, NULL, NULL, 0, rebelResource::init, 2, 11, 700, 300, },
	{ 4, 0, 0L, 0L, 0L, &rebelWidgetDialog, NULL, NULL, 0, rebelWidget::init, 2, 11, 800, 0, },
	{ 4, 0, 0L, 0L, 0L, &rebelOpenSelector, NULL, NULL, 0, NULL, 0, 1, 0, 0, },
	{ 4, 0, 0L, 0L, 0L, &rebelSaveAsSelector, NULL, NULL, 0, NULL, 0, 1, 0, 0, },
	{ 0 }
	};

static const ApEventLink_t AbLinks_rebelBaseWindow[] = {
	{ 8, 2, 110L, 2L, 0L, NULL, NULL, "rebelBaseWindow", 1010, rebelFile::newCallback, 0, 0, 0, 0, },
	{ 8, 2, 111L, 4L, 0L, NULL, NULL, "rebelBaseWindow", 1010, rebelFile::saveAsCallback, 0, 0, 0, 0, },
	{ 8, 2, 115L, 2L, 0L, NULL, NULL, "rebelBaseWindow", 1010, rebelFile::saveCallback, 0, 0, 0, 0, },
	{ 8, 2, 120L, 2L, 0L, NULL, NULL, "rebelBaseWindow", 1010, rebelEdit::cutCallback, 0, 0, 0, 0, },
	{ 8, 2, 99L, 2L, 0L, NULL, NULL, "rebelBaseWindow", 1010, rebelEdit::copyCallback, 0, 0, 0, 0, },
	{ 8, 2, 118L, 2L, 0L, NULL, NULL, "rebelBaseWindow", 1010, rebelEdit::pasteCallback, 0, 0, 0, 0, },
	{ 8, 2, 100L, 2L, 0L, NULL, NULL, "rebelBaseWindow", 1010, rebelEdit::deleteCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "rebelBaseWindow", 10003, resize, 0, 0, 0, 0, },
	{ 8, 2, 61639L, 0L, 0L, NULL, NULL, "rebelBaseWindow", 1010, rebelWin::nextButtonCallback, 0, 0, 0, 0, },
	{ 8, 2, 61638L, 0L, 0L, NULL, NULL, "rebelBaseWindow", 1010, rebelWin::prevButtonCallback, 0, 0, 0, 0, },
	{ 4, 2, 111L, 2L, 0L, &rebelOpenSelector, NULL, "rebelBaseWindow", 1010, rebelFile::initSelectorCallback, 0, 9, 0, 0, },
	{ 5, 0, 0L, 0L, 0L, &rebelFileMenu, NULL, "rebelFileMenuButton", 2007, NULL, 0, 1, 0, 0, },
	{ 5, 0, 0L, 0L, 0L, &rebelEditMenu, NULL, "rebelEditMenuButton", 2007, NULL, 0, 1, 0, 0, },
	{ 5, 0, 0L, 0L, 0L, &rebelViewMenu, NULL, "rebelViewMenuButton", 2007, NULL, 0, 1, 0, 0, },
	{ 5, 0, 0L, 0L, 0L, &rebelAppMenu, NULL, "rebelAppMenuButton", 2007, NULL, 0, 1, 0, 0, },
	{ 4, 0, 0L, 0L, 0L, &rebelOpenSelector, NULL, "rebelOpenButton", 2009, rebelFile::initSelectorCallback, 0, 1, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "rebelSaveButton", 2009, rebelFile::saveCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "rebelCutButton", 2009, rebelEdit::cutCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "rebelCopyButton", 2009, rebelEdit::copyCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "rebelPasteButton", 2009, rebelEdit::pasteCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "rebelMakeButton", 2009, rebelApp::makeAppCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "rebelBkgd", 2009, rebelApp::clearCursorCallback, 0, 0, 0, 0, },
	{ 0 }
	};

static const ApEventLink_t AbLinks_rebelWindow[] = {
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "rebelWindow", 10003, rebelWin::resizeCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "rebelWindow", 18023, rebelWin::winOpenCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "rebelWindow", 18017, rebelWin::closeWinCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "windowScrollContainer", 2009, rebelWin::addButtonCallback, 0, 0, 0, 0, },
	{ 8, 1, 0L, 0L, 544L, NULL, NULL, "windowScrollContainer", 1011, rebelWin::eventCallback, 0, 0, 0, 0, },
	{ 0 }
	};

static const ApEventLink_t AbLinks_rebelPictures[] = {
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "rebelStateButton", 2009, rebelState::resourceCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "rebelStateButton", 2007, rebelWin::initMoveCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "rebelStateButton", 2009, rebelChain::addEscapeCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "rebelStateButton", 2009, rebelWin::activeButtonCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "rebelItemButton", 2009, rebelMenuItem::resourceCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "rebelItemButton", 2007, rebelWin::initMoveCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "rebelItemButton", 2009, rebelWin::activeButtonCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "rebelFunctionButton", 2009, rebelFunctionItem::resourceCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "rebelFunctionButton", 2007, rebelWin::initMoveCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "rebelFunctionButton", 2009, rebelWin::activeButtonCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "rebelVariableButton", 2009, rebelVariableItem::resourceCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "rebelVariableButton", 2007, rebelWin::initMoveCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "rebelVariableButton", 2009, rebelWin::activeButtonCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "rebelEscapePoly", 2009, rebelState::escapeResourcesCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "rebelCommentButton", 2009, rebelComment::resourceCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "rebelCommentButton", 2009, rebelWin::activeButtonCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "rebelCommentButton", 2007, rebelWin::initMoveCallback, 0, 0, 0, 0, },
	{ 0 }
	};

static const ApEventLink_t AbLinks_rebelOpenSelector[] = {
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "rebelOpenDirText", 2009, rebelFile::setSsetDirCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "rebelOpenFileSel", 47013, rebelFile::fileNameCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "rebelOpenFileText", 2009, rebelFile::openCallback, 0, 0, 0, 0, },
	{ 9, 0, 0L, 0L, 0L, NULL, NULL, "rebelOpenFileText", 2009, NULL, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "rebelFileOpenButton", 2009, rebelFile::openCallback, 0, 0, 0, 0, },
	{ 9, 0, 0L, 0L, 0L, NULL, NULL, "rebelFileOpenButton", 2009, NULL, 0, 0, 0, 0, },
	{ 0 }
	};

static const ApEventLink_t AbLinks_rebelSaveAsSelector[] = {
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "rebelSaveDirText", 2009, rebelFile::setSsetDirCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "rebelSaveFileSel", 47013, rebelFile::fileNameCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "rebelSaveFileText", 2009, rebelFile::saveAsCallback, 0, 0, 0, 0, },
	{ 9, 0, 0L, 0L, 0L, NULL, NULL, "rebelSaveFileText", 2009, NULL, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "rebelFileSaveAsButton", 2009, rebelFile::saveAsCallback, 0, 0, 0, 0, },
	{ 9, 0, 0L, 0L, 0L, NULL, NULL, "rebelFileSaveAsButton", 2009, NULL, 0, 0, 0, 0, },
	{ 0 }
	};

static const ApEventLink_t AbLinks_rebelResourceDialog[] = {
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "rebelResourceDialog", 10003, rebelResource::resizeCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "rebelResourceDialog", 18023, rebelResource::winOpenCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "resourcePanelGroup", 64010, rebelResource::switchCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "chainNameText", 2009, rebelChain::chainNameCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "chainNameText", 4010, rebelResource::textNotify, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "chainNameText", 2010, rebelEdit::textGotFocusCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "restartText", 2009, rebelChain::restartFunctionCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "restartText", 4010, rebelResource::textNotify, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "restartText", 2010, rebelEdit::textGotFocusCallback, 0, 0, 0, 0, },
	{ 4, 0, 0L, 0L, 0L, &rebelActionDialog, NULL, "actionLabel", 2009, rebelResource::initActionCallback, 2, 9, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "stateNameText", 2009, rebelState::stateNameCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "stateNameText", 4010, rebelResource::textNotify, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "stateNameText", 2010, rebelEdit::textGotFocusCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ecodeText", 2009, rebelState::ecodeCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ecodeText", 4010, rebelResource::textNotify, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ecodeText", 2010, rebelEdit::textGotFocusCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "actionText", 2009, rebelState::rexActionCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "actionText", 4010, rebelResource::textNotify, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "actionText", 2010, rebelEdit::textGotFocusCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "runningLineText", 2009, rebelState::runLineCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "runningLineText", 4010, rebelResource::textNotify, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "runningLineText", 2010, rebelEdit::textGotFocusCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "timeText", 2009, rebelState::rexTimeCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "timeText", 4010, rebelResource::textNotify, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "timeText", 2010, rebelEdit::textGotFocusCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "randText", 2009, rebelState::rexRandCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "randText", 4010, rebelResource::textNotify, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "randText", 2010, rebelEdit::textGotFocusCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "stateCommentText", 2009, rebelState::stateCommentCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "stateCommentText", 4010, rebelResource::textNotify, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "stateCommentText", 2010, rebelEdit::textGotFocusCallback, 0, 0, 0, 0, },
	{ 4, 0, 0L, 0L, 0L, &rebelEscapeDialog, NULL, "escapeOnLabel", 2009, rebelResource::initEscapeCallback, 2, 9, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "escapeFromText", 4010, rebelResource::textNotify, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "escapeToText", 4010, rebelResource::textNotify, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "escapeOnText", 2009, rebelState::escapeOnCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "escapeOnText", 4010, rebelResource::textNotify, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "escapeOnText", 2010, rebelEdit::textGotFocusCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "escapeCommentText", 2009, rebelState::escapeCommentCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "escapeCommentText", 4010, rebelResource::textNotify, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "escapeCommentText", 2010, rebelEdit::textGotFocusCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "menuAffComboBox", 23010, rebelMenu::accessFlagCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "menuAffComboBox", 23010, rebelResource::setMenuAffText, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "menuNameText", 2009, rebelMenu::menuNameCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "menuNameText", 4010, rebelResource::textNotify, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "menuNameText", 2010, rebelEdit::textGotFocusCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "menuVariableListText", 2009, rebelMenu::variableListCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "menuVariableListText", 4010, rebelResource::textNotify, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "menuVariableListText", 2010, rebelEdit::textGotFocusCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "menuBasePtrText", 2009, rebelMenu::basePtrCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "menuBasePtrText", 4010, rebelResource::textNotify, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "menuBasePtrText", 2010, rebelEdit::textGotFocusCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "menuAccessFuncText", 2009, rebelMenu::mafCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "menuAccessFuncText", 4010, rebelResource::textNotify, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "menuAccessFuncText", 2010, rebelEdit::textGotFocusCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "menuAccessFlagText", 2009, rebelMenu::accessFlagCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "menuAccessFlagText", 4010, rebelResource::textNotify, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "menuAccessFlagText", 2010, rebelEdit::textGotFocusCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "menuRootGenText", 2009, rebelMenu::agfCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "menuRootGenText", 4010, rebelResource::textNotify, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "menuRootGenText", 2010, rebelEdit::textGotFocusCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "menuHelpMultiText", 4010, rebelMenu::helpCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "menuHelpNameText", 2009, rebelMenu::helpNameCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "menuHelpNameText", 2010, rebelEdit::textGotFocusCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "menuHelpNameText", 4010, rebelResource::textNotify, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "vlistAffComboBox", 23010, rebelMenuItem::accessFlagCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "vlistAffComboBox", 23010, rebelResource::setVlistAffText, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "vlistTypeComboBox", 2009, rebelMenuItem::varTypeCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "vlistTypeComboBox", 2009, rebelResource::setVlistTypeText, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "vlistTypeComboBox", 23010, rebelMenuItem::varTypeCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "vlistTypeComboBox", 23010, rebelResource::setVlistTypeText, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "vlistNameText", 2009, rebelMenuItem::itemNameCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "vlistNameText", 4010, rebelResource::textNotify, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "vlistNameText", 2010, rebelEdit::textGotFocusCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "vlistVariableText", 2009, rebelMenuItem::variableCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "vlistVariableText", 4010, rebelResource::textNotify, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "vlistVariableText", 2010, rebelEdit::textGotFocusCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "vlistBasePtrText", 2009, rebelMenuItem::basePtrCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "vlistBasePtrText", 4010, rebelResource::textNotify, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "vlistBasePtrText", 2010, rebelEdit::textGotFocusCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "vlistAccessFuncText", 2009, rebelMenuItem::vafCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "vlistAccessFuncText", 4010, rebelResource::textNotify, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "vlistAccessFuncText", 2010, rebelEdit::textGotFocusCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "vlistAccessFlagText", 2009, rebelMenuItem::accessFlagCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "vlistAccessFlagText", 4010, rebelResource::textNotify, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "vlistAccessFlagText", 2010, rebelEdit::textGotFocusCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "vlistTypeText", 2009, rebelMenuItem::varTypeCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "vlistTypeText", 4010, rebelResource::textNotify, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "vlistTypeText", 2010, rebelEdit::textGotFocusCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "functionLabelText", 2009, rebelFunctionItem::functionNameCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "functionLabelText", 4010, rebelResource::textNotify, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "functionLabelText", 2010, rebelEdit::textGotFocusCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "functionNameText", 2009, rebelFunctionItem::functionCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "functionNameText", 4010, rebelResource::textNotify, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "functionNameText", 2010, rebelEdit::textGotFocusCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "functionFormatText", 2009, rebelFunctionItem::formatCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "functionFormatText", 4010, rebelResource::textNotify, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "functionFormatText", 2010, rebelEdit::textGotFocusCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "variableNameText", 2009, rebelVariableItem::variableNameCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "variableNameText", 4010, rebelResource::textNotify, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "variableNameText", 2010, rebelEdit::textGotFocusCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "variableVariableText", 2009, rebelVariableItem::variableCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "variableVariableText", 4010, rebelResource::textNotify, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "variableVariableText", 2010, rebelEdit::textGotFocusCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "commentMultiText", 4010, rebelComment::textCallback, 0, 0, 0, 0, },
	{ 0 }
	};

static const ApEventLink_t AbLinks_rebelWidgetDialog[] = {
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "rebelStateWidgetButton", 2009, rebelChain::newStateCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "rebelEscapeWidgetButton", 2009, rebelChain::newEscapeCallback, 0, 0, 0, 0, },
	{ 4, 0, 0L, 0L, 0L, &rebelAbortDialog, NULL, "rebelAbortWidgetButton", 2009, rebelChain::initAbortListCallback, 2, 9, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "rebelMenuItemWidgetButton", 2009, rebelMenu::newItemCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "rebelUserFunctionWdigetButton", 2009, rebelFunction::newFunctionCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "rebelRealTimeVarWidgetButton", 2009, rebelVariable::newVariableCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "commentWidgetButton", 2009, rebelWin::newCommentCallback, 0, 0, 0, 0, },
	{ 0 }
	};

static const ApEventLink_t AbLinks_rebelActionDialog[] = {
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "actionComboBox", 2009, rebelState::actionNameCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "actionComboBox", 23010, rebelState::actionNameCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "actionComboBox", 4010, rebelResource::textNotify, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "actionComboBox", 2009, rebelResource::clearActionArgsCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "actionComboBox", 23010, rebelResource::clearActionArgsCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "arg1Text", 2009, rebelState::actionArgCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "arg1Text", 4010, rebelResource::textNotify, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "arg1Text", 2010, rebelEdit::textGotFocusCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "arg2Text", 2009, rebelState::actionArgCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "arg2Text", 4010, rebelResource::textNotify, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "arg2Text", 2010, rebelEdit::textGotFocusCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "arg3Text", 2009, rebelState::actionArgCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "arg3Text", 4010, rebelResource::textNotify, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "arg3Text", 2010, rebelEdit::textGotFocusCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "arg4Text", 2009, rebelState::actionArgCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "arg4Text", 4010, rebelResource::textNotify, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "arg4Text", 2010, rebelEdit::textGotFocusCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "arg5Text", 2009, rebelState::actionArgCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "arg5Text", 4010, rebelResource::textNotify, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "arg5Text", 2010, rebelEdit::textGotFocusCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "arg6Text", 2009, rebelState::actionArgCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "arg6Text", 4010, rebelResource::textNotify, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "arg6Text", 2010, rebelEdit::textGotFocusCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "arg7Text", 2009, rebelState::actionArgCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "arg7Text", 4010, rebelResource::textNotify, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "arg7Text", 2010, rebelEdit::textGotFocusCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "arg8Text", 2009, rebelState::actionArgCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "arg8Text", 4010, rebelResource::textNotify, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "arg8Text", 2010, rebelEdit::textGotFocusCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "arg9Text", 2009, rebelState::actionArgCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "arg9Text", 4010, rebelResource::textNotify, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "arg9Text", 2010, rebelEdit::textGotFocusCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "arg10Text", 2009, rebelState::actionArgCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "arg10Text", 4010, rebelResource::textNotify, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "arg10Text", 2010, rebelEdit::textGotFocusCallback, 0, 0, 0, 0, },
	{ 10, 0, 0L, 0L, 0L, NULL, NULL, "actionCancelButton", 2009, NULL, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "actionCancelButton", 2009, rebelState::clearActionArgsCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "actionOkButton", 2009, rebelState::rexActionOkCallback, 0, 0, 0, 0, },
	{ 9, 0, 0L, 0L, 0L, NULL, NULL, "actionOkButton", 2009, NULL, 0, 0, 0, 0, },
	{ 0 }
	};

static const ApEventLink_t AbLinks_rebelGlobalsDialog[] = {
	{ 9, 0, 0L, 0L, 0L, NULL, NULL, "globalsOkButton", 2009, NULL, 0, 0, 0, 0, },
	{ 0 }
	};

static const ApEventLink_t AbLinks_rebelEscapeDialog[] = {
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "escapeConstantText", 4010, rebelResource::textNotify, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "escapeConstantText", 2009, rebelState::escapeDialogCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "escapeConstantText", 2010, rebelEdit::textGotFocusCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "escapeVariableText", 4010, rebelResource::textNotify, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "escapeVariableText", 2009, rebelState::escapeDialogCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "escapeVariableText", 2010, rebelEdit::textGotFocusCallback, 0, 0, 0, 0, },
	{ 10, 0, 0L, 0L, 0L, NULL, NULL, "escapeCancelButton", 2009, NULL, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "escapeOkButton", 2009, rebelState::escapeOkCallback, 0, 0, 0, 0, },
	{ 9, 0, 0L, 0L, 0L, NULL, NULL, "escapeOkButton", 2009, NULL, 0, 0, 0, 0, },
	{ 0 }
	};

static const ApEventLink_t AbLinks_rebelAbortDialog[] = {
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "abortList", 23010, rebelChain::addAbortStateCallback, 0, 0, 0, 0, },
	{ 10, 0, 0L, 0L, 0L, NULL, NULL, "abortListCancelButton", 2009, NULL, 0, 0, 0, 0, },
	{ 9, 0, 0L, 0L, 0L, NULL, NULL, "abortListOkButton", 2009, NULL, 0, 0, 0, 0, },
	{ 0 }
	};

static const ApEventLink_t AbLinks_rebelLabActionsDialog[] = {
	{ 9, 0, 0L, 0L, 0L, NULL, NULL, "actionsOkButton", 2009, NULL, 0, 0, 0, 0, },
	{ 0 }
	};

static const ApEventLink_t AbLinks_rebelFileMenu[] = {
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "rebelFileMenuNew", 2009, rebelFile::newCallback, 0, 0, 0, 0, },
	{ 4, 0, 0L, 0L, 0L, &rebelOpenSelector, NULL, "rebelFileMenuOpen", 2009, rebelFile::initSelectorCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "fileMenuPrototypes", 2009, rebelFile::prototypeCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "rebelFileMenuSave", 2009, rebelFile::saveCallback, 0, 0, 0, 0, },
	{ 4, 0, 0L, 0L, 0L, &rebelSaveAsSelector, NULL, "rebelFileMenuSaveAs", 2009, rebelFile::initSelectorCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "rebelFileMenuSpot", 2009, rebelFile::saveAsSpotCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "rebelFileMenuSaveSnapshot", 2009, rebelFile::saveSnapshotCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "rebelFileMenuClose", 2009, rebelFile::closeCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "rebelFileMenuExit", 2009, rebelFile::exitCallback, 0, 0, 0, 0, },
	{ 0 }
	};

static const ApEventLink_t AbLinks_rebelEditMenu[] = {
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "rebelEditMenuCut", 2009, rebelEdit::cutCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "rebelEditMenuCopy", 2009, rebelEdit::copyCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "rebelEditMenuPaste", 2009, rebelEdit::pasteCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "rebelEditMenuSnap", 2009, rebelEdit::snapCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "rebelEditSrcJedEdit", 2009, rebelApp::jedCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "rebelEditSrcWsEdit", 2009, rebelApp::wsCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "rebelEditSrcViEdit", 2009, rebelApp::viCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "rebelEditSrcPedEdit", 2009, rebelApp::pedCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "rebelEditHdrJedEdit", 2009, rebelApp::jedHdrCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "rebelEditHdrWsEdit", 2009, rebelApp::wsHdrCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "rebelEditHdrViEdit", 2009, rebelApp::viHdrCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "rebelEditHdrPedEdit", 2009, rebelApp::pedHdrCallback, 0, 0, 0, 0, },
	{ 0 }
	};

static const ApEventLink_t AbLinks_rebelAppMenu[] = {
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "rebelAppMenuChain", 2009, rebelApp::newChainCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "rebelAppMenuMenu", 2009, rebelApp::newMenuCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "rebelAppMenuVariable", 2009, rebelApp::newVarCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "rebelAppMenuFunction", 2009, rebelApp::newFuncCallback, 0, 0, 0, 0, },
	{ 4, 0, 0L, 0L, 0L, &rebelGlobalsDialog, NULL, "rebelAppMenuGlobals", 2009, rebelApp::initGlobalListCallback, 0, 1, 0, 0, },
	{ 4, 0, 0L, 0L, 0L, &rebelLabActionsDialog, NULL, "rebelAppMenuActions", 2009, rebelApp::initActionListCallback, 0, 1, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "rebelAppMenuMake", 2009, rebelApp::makeAppCallback, 0, 0, 0, 0, },
	{ 0 }
	};

static const ApEventLink_t AbLinks_rebelViewMenu[] = {
	{ 4, 0, 0L, 0L, 0L, &rebelWidgetDialog, NULL, "rebelViewMenuWidgets", 2009, NULL, 0, 11, 0, 0, },
	{ 4, 0, 0L, 0L, 0L, &rebelResourceDialog, NULL, "rebelViewMenuResources", 2009, NULL, 0, 11, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "rebelViewMenuHide", 2009, rebelApp::hideAllCallback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "rebelViewMenuShow", 2009, rebelApp::showAllCallback, 0, 0, 0, 0, },
	{ 0 }
	};


#if defined(__cplusplus)
}
#endif

