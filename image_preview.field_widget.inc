<?php

/**
 * Implements hook_field_widget_info().
 */
function image_preview_field_widget_info() {
  return array(
    'image_preview' => array(
      'label' => t('Image preview'),
      'field types' => array('image'),
      'behaviors' => array(
        'multiple values' => FIELD_BEHAVIOR_CUSTOM,
        'default value' => FIELD_BEHAVIOR_DEFAULT,
      ),
      'settings' => array(
        'progress_indicator' => 'throbber',
        'mode' => 'ajax'
      ),
      'weight' => 2,
    )
  );
}

/**
 * Implements hook_field_widget_settings_form().
 */
function image_preview_field_widget_settings_form($field, $instance) {
  $form = array();
  $settings = &$instance['widget']['settings'];
  $form['schema'] = array(
    '#type' => 'select',
    '#title' => t('Image upload schema'),
    '#default_value' => $settings['schema'],
    '#options' => array(
      'ajax' => t('ajax upload'),
      'form' => t('form upload'),
      'auto' => t('auto upload')
    )
  );
  $form['#validate'][] = 'image_preview_field_widget_settings_form_validate';
  return $form;
}

function image_preview_field_widget_settings_form_validate($element, &$form_state, $form) {
  $settings = &$form_state['values']['instance']['widget']['settings'];
}

/**
 * Implements hook_field_widget_form().
 */
function image_preview_field_widget_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {
  $settings = $instance['settings'];
  $defaults = array();
  $element += array(
    '#type' => 'file',
    '#upload_location' => file_field_widget_uri($field, $instance),
    '#upload_validators' => file_field_widget_upload_validators($field, $instance),
    '#value_callback' => 'file_field_widget_value',
    '#process' => array(), //array_merge($element_info['#process'], array('file_field_widget_process')),
    '#progress_indicator' => $instance['widget']['settings']['progress_indicator'],
    // Allows this field to return an array instead of a single value.
    '#extended' => TRUE,
  );
  if ($field['cardinality'] == 1) {
    $elements = array($element);
  } else {
    //$items 已经存在的值
    foreach ($items as $item) {
      $elements[$delta] = $element;
      $elements[$delta]['#default_value'] = $item;
      $elements[$delta]['#weight'] = $delta;
      $delta++;
    }
    if (($field['cardinality'] == FIELD_CARDINALITY_UNLIMITED || $delta < $field['cardinality']) && empty($form_state['programmed'])) {
      $elements[$delta] = $element;
      $elements[$delta]['#default_value'] = $defaults;
      $elements[$delta]['#weight'] = $delta;
      $elements[$delta]['#required'] = ($element['#required'] && $delta == 0);
    }
  }
  $js_settings = array($instance['field_name'] => array(
      'extensions' => $settings['file_extensions'],
      'filesize' => array('max' => parse_size($settings['max_filesize'])),
      'resolution' => array('max' => explode('x', $settings['max_resolution']), 'min' => explode('x', $settings['min_resolution']))
  ));
  drupal_add_js(array('image_preview' => $js_settings), 'setting');
  $elements['#attached']['js'][] = drupal_get_path('module', 'image_preview') . '/image_preview.js';
  return $elements;
}

function image_preview_field_widget_validate() {
  
}

function image_preview_field_widget_submit() {
  
}

function image_preview_field_process($element, &$form_state, $form) {
  return $element;
}
