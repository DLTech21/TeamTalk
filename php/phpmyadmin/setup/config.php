<?php
/* vim: set expandtab sw=4 ts=4 sts=4: */
/**
 * Front controller for config view / download and clear
 *
 * @package PhpMyAdmin-Setup
 */
use PMA\libraries\config\FormDisplay;
use PMA\setup\lib\ConfigGenerator;

/**
 * Core libraries.
 */
require './lib/common.inc.php';

require './libraries/config/setup.forms.php';

$form_display = new FormDisplay($GLOBALS['ConfigFile']);
$form_display->registerForm('_config.php', $forms['_config.php']);
$form_display->save('_config.php');
$config_file_path = $GLOBALS['ConfigFile']->getFilePath();

if (isset($_POST['eol'])) {
    $_SESSION['eol'] = ($_POST['eol'] == 'unix') ? 'unix' : 'win';
}

if (PMA_ifSetOr($_POST['submit_clear'], '')) {
    //
    // Clear current config and return to main page
    //
    $GLOBALS['ConfigFile']->resetConfigData();
    // drop post data
    header('HTTP/1.1 303 See Other');
    header('Location: index.php' . PMA_URL_getCommon());
    exit;
} elseif (PMA_ifSetOr($_POST['submit_download'], '')) {
    //
    // Output generated config file
    //
    PMA_downloadHeader('config.inc.php', 'text/plain');
    echo ConfigGenerator::getConfigFile($GLOBALS['ConfigFile']);
    exit;
} elseif (PMA_ifSetOr($_POST['submit_save'], '')) {
    //
    // Save generated config file on the server
    //
    file_put_contents(
        $config_file_path,
        ConfigGenerator::getConfigFile($GLOBALS['ConfigFile'])
    );
    header('HTTP/1.1 303 See Other');
    header('Location: index.php' . PMA_URL_getCommon() . '&action_done=config_saved');
    exit;
} elseif (PMA_ifSetOr($_POST['submit_load'], '')) {
    //
    // Load config file from the server
    //
    $cfg = array();
    include_once $config_file_path;
    $GLOBALS['ConfigFile']->setConfigData($cfg);
    header('HTTP/1.1 303 See Other');
    header('Location: index.php' . PMA_URL_getCommon());
    exit;
} elseif (PMA_ifSetOr($_POST['submit_delete'], '')) {
    //
    // Delete config file on the server
    //
    @unlink($config_file_path);
    header('HTTP/1.1 303 See Other');
    header('Location: index.php' . PMA_URL_getCommon());
    exit;
} else {
    //
    // Show generated config file in a <textarea>
    //
    header('HTTP/1.1 303 See Other');
    header('Location: index.php' . PMA_URL_getCommon() . '&page=config');
    exit;
}
