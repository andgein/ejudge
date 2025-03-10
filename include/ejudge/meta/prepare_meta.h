// This is an auto-generated file, do not edit

#ifndef __PREPARE_META_H__
#define __PREPARE_META_H__

#include <stdlib.h>

enum
{
  CNTSGLOB_sleep_time = 1,
  CNTSGLOB_serve_sleep_time,
  CNTSGLOB_contest_time,
  CNTSGLOB_max_run_size,
  CNTSGLOB_max_run_total,
  CNTSGLOB_max_run_num,
  CNTSGLOB_max_clar_size,
  CNTSGLOB_max_clar_total,
  CNTSGLOB_max_clar_num,
  CNTSGLOB_board_fog_time,
  CNTSGLOB_board_unfog_time,
  CNTSGLOB_autoupdate_standings,
  CNTSGLOB_use_ac_not_ok,
  CNTSGLOB_inactivity_timeout,
  CNTSGLOB_disable_auto_testing,
  CNTSGLOB_disable_testing,
  CNTSGLOB_enable_runlog_merge,
  CNTSGLOB_secure_run,
  CNTSGLOB_detect_violations,
  CNTSGLOB_enable_memory_limit_error,
  CNTSGLOB_advanced_layout,
  CNTSGLOB_uuid_run_store,
  CNTSGLOB_enable_32bit_checkers,
  CNTSGLOB_ignore_bom,
  CNTSGLOB_disable_user_database,
  CNTSGLOB_enable_max_stack_size,
  CNTSGLOB_time_limit_retry_count,
  CNTSGLOB_score_n_best_problems,
  CNTSGLOB_require_problem_uuid,
  CNTSGLOB_stand_ignore_after,
  CNTSGLOB_contest_finish_time,
  CNTSGLOB_appeal_deadline,
  CNTSGLOB_fog_standings_updated,
  CNTSGLOB_start_standings_updated,
  CNTSGLOB_unfog_standings_updated,
  CNTSGLOB_team_enable_src_view,
  CNTSGLOB_team_enable_rep_view,
  CNTSGLOB_team_enable_ce_view,
  CNTSGLOB_team_show_judge_report,
  CNTSGLOB_disable_clars,
  CNTSGLOB_disable_team_clars,
  CNTSGLOB_disable_submit_after_ok,
  CNTSGLOB_ignore_compile_errors,
  CNTSGLOB_enable_continue,
  CNTSGLOB_enable_report_upload,
  CNTSGLOB_priority_adjustment,
  CNTSGLOB_ignore_success_time,
  CNTSGLOB_disable_failed_test_view,
  CNTSGLOB_always_show_problems,
  CNTSGLOB_disable_user_standings,
  CNTSGLOB_disable_language,
  CNTSGLOB_problem_navigation,
  CNTSGLOB_problem_tab_size,
  CNTSGLOB_vertical_navigation,
  CNTSGLOB_disable_virtual_start,
  CNTSGLOB_disable_virtual_auto_judge,
  CNTSGLOB_enable_auto_print_protocol,
  CNTSGLOB_notify_clar_reply,
  CNTSGLOB_notify_status_change,
  CNTSGLOB_memoize_user_results,
  CNTSGLOB_disable_auto_refresh,
  CNTSGLOB_enable_eoln_select,
  CNTSGLOB_start_on_first_login,
  CNTSGLOB_enable_virtual_restart,
  CNTSGLOB_name,
  CNTSGLOB_root_dir,
  CNTSGLOB_serve_socket,
  CNTSGLOB_enable_l10n,
  CNTSGLOB_l10n_dir,
  CNTSGLOB_standings_locale,
  CNTSGLOB_standings_locale_id,
  CNTSGLOB_checker_locale,
  CNTSGLOB_contest_id,
  CNTSGLOB_socket_path,
  CNTSGLOB_contests_dir,
  CNTSGLOB_lang_config_dir,
  CNTSGLOB_charset,
  CNTSGLOB_standings_charset,
  CNTSGLOB_stand2_charset,
  CNTSGLOB_plog_charset,
  CNTSGLOB_conf_dir,
  CNTSGLOB_problems_dir,
  CNTSGLOB_script_dir,
  CNTSGLOB_test_dir,
  CNTSGLOB_corr_dir,
  CNTSGLOB_info_dir,
  CNTSGLOB_tgz_dir,
  CNTSGLOB_checker_dir,
  CNTSGLOB_statement_dir,
  CNTSGLOB_plugin_dir,
  CNTSGLOB_test_sfx,
  CNTSGLOB_corr_sfx,
  CNTSGLOB_info_sfx,
  CNTSGLOB_tgz_sfx,
  CNTSGLOB_tgzdir_sfx,
  CNTSGLOB_ejudge_checkers_dir,
  CNTSGLOB_contest_start_cmd,
  CNTSGLOB_contest_stop_cmd,
  CNTSGLOB_description_file,
  CNTSGLOB_contest_plugin_file,
  CNTSGLOB_virtual_end_options,
  CNTSGLOB_virtual_end_info,
  CNTSGLOB_super_run_dir,
  CNTSGLOB_test_pat,
  CNTSGLOB_corr_pat,
  CNTSGLOB_info_pat,
  CNTSGLOB_tgz_pat,
  CNTSGLOB_tgzdir_pat,
  CNTSGLOB_clardb_plugin,
  CNTSGLOB_rundb_plugin,
  CNTSGLOB_xuser_plugin,
  CNTSGLOB_status_plugin,
  CNTSGLOB_var_dir,
  CNTSGLOB_run_log_file,
  CNTSGLOB_clar_log_file,
  CNTSGLOB_archive_dir,
  CNTSGLOB_clar_archive_dir,
  CNTSGLOB_run_archive_dir,
  CNTSGLOB_report_archive_dir,
  CNTSGLOB_team_report_archive_dir,
  CNTSGLOB_xml_report_archive_dir,
  CNTSGLOB_full_archive_dir,
  CNTSGLOB_audit_log_dir,
  CNTSGLOB_uuid_archive_dir,
  CNTSGLOB_team_extra_dir,
  CNTSGLOB_legacy_status_dir,
  CNTSGLOB_work_dir,
  CNTSGLOB_print_work_dir,
  CNTSGLOB_diff_work_dir,
  CNTSGLOB_a2ps_path,
  CNTSGLOB_a2ps_args,
  CNTSGLOB_lpr_path,
  CNTSGLOB_lpr_args,
  CNTSGLOB_diff_path,
  CNTSGLOB_compile_dir,
  CNTSGLOB_compile_queue_dir,
  CNTSGLOB_compile_src_dir,
  CNTSGLOB_extra_compile_dirs,
  CNTSGLOB_compile_out_dir,
  CNTSGLOB_compile_status_dir,
  CNTSGLOB_compile_report_dir,
  CNTSGLOB_compile_work_dir,
  CNTSGLOB_run_dir,
  CNTSGLOB_run_queue_dir,
  CNTSGLOB_run_exe_dir,
  CNTSGLOB_run_out_dir,
  CNTSGLOB_run_status_dir,
  CNTSGLOB_run_report_dir,
  CNTSGLOB_run_team_report_dir,
  CNTSGLOB_run_full_archive_dir,
  CNTSGLOB_run_work_dir,
  CNTSGLOB_run_check_dir,
  CNTSGLOB_htdocs_dir,
  CNTSGLOB_score_system,
  CNTSGLOB_tests_to_accept,
  CNTSGLOB_is_virtual,
  CNTSGLOB_prune_empty_users,
  CNTSGLOB_rounding_mode,
  CNTSGLOB_max_file_length,
  CNTSGLOB_max_line_length,
  CNTSGLOB_max_cmd_length,
  CNTSGLOB_team_info_url,
  CNTSGLOB_prob_info_url,
  CNTSGLOB_standings_file_name,
  CNTSGLOB_stand_header_file,
  CNTSGLOB_stand_footer_file,
  CNTSGLOB_stand_symlink_dir,
  CNTSGLOB_users_on_page,
  CNTSGLOB_stand_file_name_2,
  CNTSGLOB_stand_fancy_style,
  CNTSGLOB_stand_extra_format,
  CNTSGLOB_stand_extra_legend,
  CNTSGLOB_stand_extra_attr,
  CNTSGLOB_stand_table_attr,
  CNTSGLOB_stand_place_attr,
  CNTSGLOB_stand_team_attr,
  CNTSGLOB_stand_prob_attr,
  CNTSGLOB_stand_solved_attr,
  CNTSGLOB_stand_score_attr,
  CNTSGLOB_stand_penalty_attr,
  CNTSGLOB_stand_time_attr,
  CNTSGLOB_stand_self_row_attr,
  CNTSGLOB_stand_r_row_attr,
  CNTSGLOB_stand_v_row_attr,
  CNTSGLOB_stand_u_row_attr,
  CNTSGLOB_stand_success_attr,
  CNTSGLOB_stand_fail_attr,
  CNTSGLOB_stand_trans_attr,
  CNTSGLOB_stand_disq_attr,
  CNTSGLOB_stand_use_login,
  CNTSGLOB_stand_show_avatar,
  CNTSGLOB_stand_show_first_solver,
  CNTSGLOB_stand_show_ok_time,
  CNTSGLOB_stand_show_att_num,
  CNTSGLOB_stand_sort_by_solved,
  CNTSGLOB_stand_row_attr,
  CNTSGLOB_stand_page_table_attr,
  CNTSGLOB_stand_page_row_attr,
  CNTSGLOB_stand_page_col_attr,
  CNTSGLOB_stand_page_cur_attr,
  CNTSGLOB_stand_collate_name,
  CNTSGLOB_stand_enable_penalty,
  CNTSGLOB_stand_header_txt,
  CNTSGLOB_stand_footer_txt,
  CNTSGLOB_stand2_file_name,
  CNTSGLOB_stand2_header_file,
  CNTSGLOB_stand2_footer_file,
  CNTSGLOB_stand2_header_txt,
  CNTSGLOB_stand2_footer_txt,
  CNTSGLOB_stand2_symlink_dir,
  CNTSGLOB_plog_file_name,
  CNTSGLOB_plog_header_file,
  CNTSGLOB_plog_footer_file,
  CNTSGLOB_plog_header_txt,
  CNTSGLOB_plog_footer_txt,
  CNTSGLOB_plog_update_time,
  CNTSGLOB_plog_symlink_dir,
  CNTSGLOB_internal_xml_update_time,
  CNTSGLOB_external_xml_update_time,
  CNTSGLOB_user_exam_protocol_header_file,
  CNTSGLOB_user_exam_protocol_footer_file,
  CNTSGLOB_user_exam_protocol_header_txt,
  CNTSGLOB_user_exam_protocol_footer_txt,
  CNTSGLOB_prob_exam_protocol_header_file,
  CNTSGLOB_prob_exam_protocol_footer_file,
  CNTSGLOB_prob_exam_protocol_header_txt,
  CNTSGLOB_prob_exam_protocol_footer_txt,
  CNTSGLOB_full_exam_protocol_header_file,
  CNTSGLOB_full_exam_protocol_footer_file,
  CNTSGLOB_full_exam_protocol_header_txt,
  CNTSGLOB_full_exam_protocol_footer_txt,
  CNTSGLOB_extended_sound,
  CNTSGLOB_disable_sound,
  CNTSGLOB_sound_player,
  CNTSGLOB_accept_sound,
  CNTSGLOB_runtime_sound,
  CNTSGLOB_timelimit_sound,
  CNTSGLOB_presentation_sound,
  CNTSGLOB_wrong_sound,
  CNTSGLOB_internal_sound,
  CNTSGLOB_start_sound,
  CNTSGLOB_team_download_time,
  CNTSGLOB_cr_serialization_key,
  CNTSGLOB_show_astr_time,
  CNTSGLOB_ignore_duplicated_runs,
  CNTSGLOB_report_error_code,
  CNTSGLOB_auto_short_problem_name,
  CNTSGLOB_compile_real_time_limit,
  CNTSGLOB_checker_real_time_limit,
  CNTSGLOB_show_deadline,
  CNTSGLOB_separate_user_score,
  CNTSGLOB_show_sha1,
  CNTSGLOB_show_judge_identity,
  CNTSGLOB_use_gzip,
  CNTSGLOB_min_gzip_size,
  CNTSGLOB_use_dir_hierarchy,
  CNTSGLOB_html_report,
  CNTSGLOB_xml_report,
  CNTSGLOB_enable_full_archive,
  CNTSGLOB_cpu_bogomips,
  CNTSGLOB_skip_full_testing,
  CNTSGLOB_skip_accept_testing,
  CNTSGLOB_enable_problem_history,
  CNTSGLOB_variant_map_file,
  CNTSGLOB_variant_map,
  CNTSGLOB_enable_printing,
  CNTSGLOB_disable_banner_page,
  CNTSGLOB_printout_uses_login,
  CNTSGLOB_team_page_quota,
  CNTSGLOB_compile_max_vm_size,
  CNTSGLOB_compile_max_stack_size,
  CNTSGLOB_compile_max_file_size,
  CNTSGLOB_user_priority_adjustments,
  CNTSGLOB_user_adjustment_info,
  CNTSGLOB_user_adjustment_map,
  CNTSGLOB_contestant_status_num,
  CNTSGLOB_contestant_status_legend,
  CNTSGLOB_contestant_status_row_attr,
  CNTSGLOB_stand_show_contestant_status,
  CNTSGLOB_stand_show_warn_number,
  CNTSGLOB_stand_contestant_status_attr,
  CNTSGLOB_stand_warn_number_attr,
  CNTSGLOB_load_user_group,
  CNTSGLOB_tokens,
  CNTSGLOB_token_info,
  CNTSGLOB_enable_tokens,
  CNTSGLOB_dates_config_file,
  CNTSGLOB_dates_config,
  CNTSGLOB_unhandled_vars,
  CNTSGLOB_disable_prob_long_name,
  CNTSGLOB_disable_passed_tests,

  CNTSGLOB_LAST_FIELD,
};

struct section_global_data;

int cntsglob_get_type(int tag);
size_t cntsglob_get_size(int tag);
const char *cntsglob_get_name(int tag);
const void *cntsglob_get_ptr(const struct section_global_data *ptr, int tag);
void *cntsglob_get_ptr_nc(struct section_global_data *ptr, int tag);
int cntsglob_lookup_field(const char *name);

struct meta_methods;
extern const struct meta_methods cntsglob_methods;


enum
{
  CNTSPROB_id = 1,
  CNTSPROB_tester_id,
  CNTSPROB_type,
  CNTSPROB_variant_num,
  CNTSPROB_full_score,
  CNTSPROB_full_user_score,
  CNTSPROB_min_score_1,
  CNTSPROB_min_score_2,
  CNTSPROB_super,
  CNTSPROB_short_name,
  CNTSPROB_abstract,
  CNTSPROB_manual_checking,
  CNTSPROB_check_presentation,
  CNTSPROB_scoring_checker,
  CNTSPROB_enable_checker_token,
  CNTSPROB_interactive_valuer,
  CNTSPROB_disable_pe,
  CNTSPROB_disable_wtl,
  CNTSPROB_wtl_is_cf,
  CNTSPROB_use_stdin,
  CNTSPROB_use_stdout,
  CNTSPROB_combined_stdin,
  CNTSPROB_combined_stdout,
  CNTSPROB_binary_input,
  CNTSPROB_binary,
  CNTSPROB_ignore_exit_code,
  CNTSPROB_olympiad_mode,
  CNTSPROB_score_latest,
  CNTSPROB_score_latest_or_unmarked,
  CNTSPROB_score_latest_marked,
  CNTSPROB_score_tokenized,
  CNTSPROB_use_ac_not_ok,
  CNTSPROB_ignore_prev_ac,
  CNTSPROB_team_enable_rep_view,
  CNTSPROB_team_enable_ce_view,
  CNTSPROB_team_show_judge_report,
  CNTSPROB_show_checker_comment,
  CNTSPROB_ignore_compile_errors,
  CNTSPROB_variable_full_score,
  CNTSPROB_ignore_penalty,
  CNTSPROB_use_corr,
  CNTSPROB_use_info,
  CNTSPROB_use_tgz,
  CNTSPROB_accept_partial,
  CNTSPROB_disable_user_submit,
  CNTSPROB_disable_tab,
  CNTSPROB_unrestricted_statement,
  CNTSPROB_restricted_statement,
  CNTSPROB_enable_submit_after_reject,
  CNTSPROB_hide_file_names,
  CNTSPROB_hide_real_time_limit,
  CNTSPROB_enable_tokens,
  CNTSPROB_tokens_for_user_ac,
  CNTSPROB_disable_submit_after_ok,
  CNTSPROB_disable_auto_testing,
  CNTSPROB_disable_testing,
  CNTSPROB_enable_compilation,
  CNTSPROB_skip_testing,
  CNTSPROB_hidden,
  CNTSPROB_stand_hide_time,
  CNTSPROB_advance_to_next,
  CNTSPROB_disable_ctrl_chars,
  CNTSPROB_enable_text_form,
  CNTSPROB_stand_ignore_score,
  CNTSPROB_stand_last_column,
  CNTSPROB_disable_security,
  CNTSPROB_enable_suid_run,
  CNTSPROB_enable_container,
  CNTSPROB_valuer_sets_marked,
  CNTSPROB_ignore_unmarked,
  CNTSPROB_disable_stderr,
  CNTSPROB_enable_process_group,
  CNTSPROB_enable_kill_all,
  CNTSPROB_hide_variant,
  CNTSPROB_enable_testlib_mode,
  CNTSPROB_autoassign_variants,
  CNTSPROB_require_any,
  CNTSPROB_enable_extended_info,
  CNTSPROB_stop_on_first_fail,
  CNTSPROB_enable_control_socket,
  CNTSPROB_enable_multi_header,
  CNTSPROB_use_lang_multi_header,
  CNTSPROB_notify_on_submit,
  CNTSPROB_examinator_num,
  CNTSPROB_real_time_limit,
  CNTSPROB_time_limit,
  CNTSPROB_time_limit_millis,
  CNTSPROB_test_score,
  CNTSPROB_run_penalty,
  CNTSPROB_acm_run_penalty,
  CNTSPROB_disqualified_penalty,
  CNTSPROB_compile_error_penalty,
  CNTSPROB_tests_to_accept,
  CNTSPROB_min_tests_to_accept,
  CNTSPROB_checker_real_time_limit,
  CNTSPROB_checker_time_limit_ms,
  CNTSPROB_priority_adjustment,
  CNTSPROB_score_multiplier,
  CNTSPROB_prev_runs_to_show,
  CNTSPROB_max_user_run_count,
  CNTSPROB_long_name,
  CNTSPROB_stand_name,
  CNTSPROB_stand_column,
  CNTSPROB_group_name,
  CNTSPROB_internal_name,
  CNTSPROB_uuid,
  CNTSPROB_problem_dir,
  CNTSPROB_test_dir,
  CNTSPROB_test_sfx,
  CNTSPROB_corr_dir,
  CNTSPROB_corr_sfx,
  CNTSPROB_info_dir,
  CNTSPROB_info_sfx,
  CNTSPROB_tgz_dir,
  CNTSPROB_tgz_sfx,
  CNTSPROB_tgzdir_sfx,
  CNTSPROB_input_file,
  CNTSPROB_output_file,
  CNTSPROB_test_score_list,
  CNTSPROB_tokens,
  CNTSPROB_umask,
  CNTSPROB_ok_status,
  CNTSPROB_header_pat,
  CNTSPROB_footer_pat,
  CNTSPROB_compiler_env_pat,
  CNTSPROB_container_options,
  CNTSPROB_token_info,
  CNTSPROB_score_tests,
  CNTSPROB_standard_checker,
  CNTSPROB_spelling,
  CNTSPROB_statement_file,
  CNTSPROB_alternatives_file,
  CNTSPROB_plugin_file,
  CNTSPROB_xml_file,
  CNTSPROB_stand_attr,
  CNTSPROB_source_header,
  CNTSPROB_source_footer,
  CNTSPROB_interactor_time_limit,
  CNTSPROB_interactor_real_time_limit,
  CNTSPROB_test_pat,
  CNTSPROB_corr_pat,
  CNTSPROB_info_pat,
  CNTSPROB_tgz_pat,
  CNTSPROB_tgzdir_pat,
  CNTSPROB_ntests,
  CNTSPROB_tscores,
  CNTSPROB_x_score_tests,
  CNTSPROB_test_sets,
  CNTSPROB_ts_total,
  CNTSPROB_ts_infos,
  CNTSPROB_normalization,
  CNTSPROB_normalization_val,
  CNTSPROB_deadline,
  CNTSPROB_start_date,
  CNTSPROB_date_penalty,
  CNTSPROB_dp_total,
  CNTSPROB_dp_infos,
  CNTSPROB_group_start_date,
  CNTSPROB_group_deadline,
  CNTSPROB_gsd,
  CNTSPROB_gdl,
  CNTSPROB_disable_language,
  CNTSPROB_enable_language,
  CNTSPROB_require,
  CNTSPROB_provide_ok,
  CNTSPROB_allow_ip,
  CNTSPROB_lang_compiler_env,
  CNTSPROB_lang_compiler_container_options,
  CNTSPROB_checker_env,
  CNTSPROB_valuer_env,
  CNTSPROB_interactor_env,
  CNTSPROB_style_checker_env,
  CNTSPROB_test_checker_env,
  CNTSPROB_init_env,
  CNTSPROB_start_env,
  CNTSPROB_check_cmd,
  CNTSPROB_valuer_cmd,
  CNTSPROB_interactor_cmd,
  CNTSPROB_style_checker_cmd,
  CNTSPROB_test_checker_cmd,
  CNTSPROB_init_cmd,
  CNTSPROB_start_cmd,
  CNTSPROB_solution_src,
  CNTSPROB_solution_cmd,
  CNTSPROB_lang_time_adj,
  CNTSPROB_lang_time_adj_millis,
  CNTSPROB_super_run_dir,
  CNTSPROB_lang_max_vm_size,
  CNTSPROB_lang_max_stack_size,
  CNTSPROB_lang_max_rss_size,
  CNTSPROB_statement_env,
  CNTSPROB_alternative,
  CNTSPROB_personal_deadline,
  CNTSPROB_pd_total,
  CNTSPROB_pd_infos,
  CNTSPROB_score_bonus,
  CNTSPROB_score_bonus_total,
  CNTSPROB_score_bonus_val,
  CNTSPROB_open_tests,
  CNTSPROB_open_tests_count,
  CNTSPROB_open_tests_val,
  CNTSPROB_final_open_tests,
  CNTSPROB_final_open_tests_count,
  CNTSPROB_final_open_tests_val,
  CNTSPROB_token_open_tests,
  CNTSPROB_token_open_tests_count,
  CNTSPROB_token_open_tests_val,
  CNTSPROB_max_vm_size,
  CNTSPROB_max_data_size,
  CNTSPROB_max_stack_size,
  CNTSPROB_max_rss_size,
  CNTSPROB_max_core_size,
  CNTSPROB_max_file_size,
  CNTSPROB_checker_max_vm_size,
  CNTSPROB_checker_max_stack_size,
  CNTSPROB_checker_max_rss_size,
  CNTSPROB_max_open_file_count,
  CNTSPROB_max_process_count,
  CNTSPROB_extid,
  CNTSPROB_unhandled_vars,
  CNTSPROB_score_view,
  CNTSPROB_score_view_score,
  CNTSPROB_score_view_text,
  CNTSPROB_xml_file_path,
  CNTSPROB_var_xml_file_paths,

  CNTSPROB_LAST_FIELD,
};

struct section_problem_data;

int cntsprob_get_type(int tag);
size_t cntsprob_get_size(int tag);
const char *cntsprob_get_name(int tag);
const void *cntsprob_get_ptr(const struct section_problem_data *ptr, int tag);
void *cntsprob_get_ptr_nc(struct section_problem_data *ptr, int tag);
int cntsprob_lookup_field(const char *name);

struct meta_methods;
extern const struct meta_methods cntsprob_methods;


enum
{
  CNTSLANG_id = 1,
  CNTSLANG_compile_id,
  CNTSLANG_disabled,
  CNTSLANG_compile_real_time_limit,
  CNTSLANG_binary,
  CNTSLANG_priority_adjustment,
  CNTSLANG_insecure,
  CNTSLANG_disable_security,
  CNTSLANG_enable_suid_run,
  CNTSLANG_is_dos,
  CNTSLANG_short_name,
  CNTSLANG_long_name,
  CNTSLANG_key,
  CNTSLANG_arch,
  CNTSLANG_src_sfx,
  CNTSLANG_exe_sfx,
  CNTSLANG_content_type,
  CNTSLANG_cmd,
  CNTSLANG_style_checker_cmd,
  CNTSLANG_style_checker_env,
  CNTSLANG_extid,
  CNTSLANG_super_run_dir,
  CNTSLANG_disable_auto_testing,
  CNTSLANG_disable_testing,
  CNTSLANG_max_vm_size,
  CNTSLANG_max_stack_size,
  CNTSLANG_max_file_size,
  CNTSLANG_run_max_stack_size,
  CNTSLANG_run_max_vm_size,
  CNTSLANG_run_max_rss_size,
  CNTSLANG_compile_dir_index,
  CNTSLANG_compile_dir,
  CNTSLANG_compile_queue_dir,
  CNTSLANG_compile_src_dir,
  CNTSLANG_compile_out_dir,
  CNTSLANG_compile_status_dir,
  CNTSLANG_compile_report_dir,
  CNTSLANG_compiler_env,
  CNTSLANG_compile_server_id,
  CNTSLANG_multi_header_suffix,
  CNTSLANG_container_options,
  CNTSLANG_unhandled_vars,
  CNTSLANG_disabled_by_config,

  CNTSLANG_LAST_FIELD,
};

struct section_language_data;

int cntslang_get_type(int tag);
size_t cntslang_get_size(int tag);
const char *cntslang_get_name(int tag);
const void *cntslang_get_ptr(const struct section_language_data *ptr, int tag);
void *cntslang_get_ptr_nc(struct section_language_data *ptr, int tag);
int cntslang_lookup_field(const char *name);

struct meta_methods;
extern const struct meta_methods cntslang_methods;


enum
{
  CNTSTESTER_id = 1,
  CNTSTESTER_name,
  CNTSTESTER_problem,
  CNTSTESTER_problem_name,
  CNTSTESTER_any,
  CNTSTESTER_is_dos,
  CNTSTESTER_no_redirect,
  CNTSTESTER_priority_adjustment,
  CNTSTESTER_ignore_stderr,
  CNTSTESTER_arch,
  CNTSTESTER_key,
  CNTSTESTER_memory_limit_type,
  CNTSTESTER_secure_exec_type,
  CNTSTESTER_abstract,
  CNTSTESTER_super,
  CNTSTESTER_is_processed,
  CNTSTESTER_skip_testing,
  CNTSTESTER_no_core_dump,
  CNTSTESTER_enable_memory_limit_error,
  CNTSTESTER_kill_signal,
  CNTSTESTER_max_stack_size,
  CNTSTESTER_max_data_size,
  CNTSTESTER_max_vm_size,
  CNTSTESTER_max_rss_size,
  CNTSTESTER_clear_env,
  CNTSTESTER_time_limit_adjustment,
  CNTSTESTER_time_limit_adj_millis,
  CNTSTESTER_run_dir,
  CNTSTESTER_run_queue_dir,
  CNTSTESTER_run_exe_dir,
  CNTSTESTER_run_out_dir,
  CNTSTESTER_run_status_dir,
  CNTSTESTER_run_report_dir,
  CNTSTESTER_run_team_report_dir,
  CNTSTESTER_run_full_archive_dir,
  CNTSTESTER_check_dir,
  CNTSTESTER_errorcode_file,
  CNTSTESTER_error_file,
  CNTSTESTER_prepare_cmd,
  CNTSTESTER_start_cmd,
  CNTSTESTER_nwrun_spool_dir,
  CNTSTESTER_start_env,
  CNTSTESTER_memory_limit_type_val,
  CNTSTESTER_secure_exec_type_val,

  CNTSTESTER_LAST_FIELD,
};

struct section_tester_data;

int cntstester_get_type(int tag);
size_t cntstester_get_size(int tag);
const char *cntstester_get_name(int tag);
const void *cntstester_get_ptr(const struct section_tester_data *ptr, int tag);
void *cntstester_get_ptr_nc(struct section_tester_data *ptr, int tag);
int cntstester_lookup_field(const char *name);

struct meta_methods;
extern const struct meta_methods cntstester_methods;

#endif
