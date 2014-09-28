<?php

/**
 * Implements hook_field_widget_info().
 */
function image_preview_widget_field_widget_info() {
  $field_types = array(
    'image',
  );
  $settings = array(
    'progress_indicator' => 'throbber',
    'schema' => 'form'
  );
  return array(
    'image_preview' => array(
      'label' => t('Image preview'),
      'field types' => $field_types,
      'behaviors' => array(
        'multiple values' => FIELD_BEHAVIOR_CUSTOM,
        'default value' => FIELD_BEHAVIOR_DEFAULT,
      ),
      'settings' => $settings,
      'weight' => 2,
    )
  );
}

/**
 * Implements hook_field_widget_settings_form().
 */
function image_preview_widget_field_widget_settings_form($field, $instance) {
  $form = array();
  $settings = &$instance['widget']['settings'];
  $form['schema'] = array(
    '#type' => 'select',
    '#title' => t('Image upload schema'),
    '#default_value' => $settings['schema'],
    '#options' => array(
      'form' => t('form upload'),
      'ajax' => t('ajax upload')
    )
  );
  $form['#validate'][] = 'image_preview_widget_field_widget_settings_form_validate';
  return $form;
}

function image_preview_widget_field_widget_settings_form_validate($element, &$form_state, $form) {
  $settings = &$form_state['values']['instance']['widget']['settings'];
}

/**
 * Implements hook_field_widget_form().
 */
function image_preview_widget_field_widget_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {
  $settings = $instance['settings'];
  $elements = image_field_widget_form($form, $form_state, $field, $instance, $langcode, $items, $delta, $element);
  foreach (element_children($elements) as $delta) {
    $elements[$delta]['#process'][] = 'image_preview_widget_field_widget_process';
  }
  $elements['#attached']['js'][] = drupal_get_path('module', 'image_preview_widget') . '/image_preview_widget.js';
  return $elements;
}

function image_preview_widget_field_widget_process($element, &$form_state, $form) {

  $item = $element['#value'];
  $item['fid'] = $element['fid']['#value'];

  $instance = field_widget_instance($element, $form_state);

  $settings = $instance['settings'];
  $widget_settings = $instance['widget']['settings'];

  $element['#theme'] = 'image_preview';

  // Add the additional alt and title fields.
  $element['alt'] = array(
    '#title' => t('Alternate text'),
    '#type' => 'textfield',
    '#default_value' => isset($item['alt']) ? $item['alt'] : '',
    '#description' => t('This text will be used by screen readers, search engines, or when the image cannot be loaded.'),
    // @see http://www.gawds.org/show.php?contentid=28
    '#maxlength' => 512,
    '#weight' => -2,
    '#access' => (bool) $item['fid'] && $settings['alt_field'],
  );
  $element['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#default_value' => isset($item['title']) ? $item['title'] : '',
    '#description' => t('The title is used as a tool tip when the user hovers the mouse over the image.'),
    '#maxlength' => 1024,
    '#weight' => -1,
    '#access' => (bool) $item['fid'] && $settings['title_field'],
  );

  return $element;
}

function image_preview_process(&$variables) {
  
}

function theme_image_preview($variables) {
  $element = $variables['element'];
  $output = '';
  $output .= '<div class="image-preview-widget form-managed-file clearfix">';

  $output .= '<div class="image-preview-upload">';
  if ($element['fid']['#value'] != 0) {
    $element['filename']['#markup'] .= ' <span class="file-size">(' . format_size($element['#file']->filesize) . ')</span> ';
  }
  $output .= drupal_render_children($element);
  $output .= '</div>';
  $output .= '<div class="image-preview">';
  $output .= drupal_render($element['preview']);
  $output .= '</div>';
  $output .= '</div>';

  return $output;
}
