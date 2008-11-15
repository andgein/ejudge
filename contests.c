/* -*- mode: c -*- */
/* $Id$ */

/* Copyright (C) 2002-2008 Alexander Chernov <cher@ejudge.ru> */

/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include "config.h"

#include "ej_limits.h"
#include "contests.h"
#include "pathutl.h"
#include "errlog.h"
#include "userlist.h"
#include "xml_utils.h"
#include "misctext.h"
#include "fileutl.h"
#include "xml_utils.h"
#include "l10n.h"
#include "ejudge_cfg.h"
#include "contests_meta.h"

#include <reuse/logger.h>
#include <reuse/xalloc.h>
#include <reuse/osdeps.h>

#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <ctype.h>
#include <limits.h>
#include <errno.h>

#if defined EJUDGE_CHARSET
#define INTERNAL_CHARSET EJUDGE_CHARSET
#else
#define INTERNAL_CHARSET "utf-8"
#endif

#define CONTEST_CHECK_TIME 5

static const int tag_to_contest_desc_map[CONTEST_LAST_TAG] =
{
  [CONTEST_REGISTER_ACCESS] = CNTS_register_access,
  [CONTEST_USERS_ACCESS] = CNTS_users_access,
  [CONTEST_MASTER_ACCESS] = CNTS_master_access,
  [CONTEST_JUDGE_ACCESS] = CNTS_judge_access,
  [CONTEST_TEAM_ACCESS] = CNTS_team_access,
  [CONTEST_SERVE_CONTROL_ACCESS] = CNTS_serve_control_access,
  [CONTEST_NAME] = CNTS_name,
  [CONTEST_NAME_EN] = CNTS_name_en,
  [CONTEST_MAIN_URL] = CNTS_main_url,
  [CONTEST_KEYWORDS] = CNTS_keywords,
  [CONTEST_USERS_HEADER_FILE] = CNTS_users_header_file,
  [CONTEST_USERS_FOOTER_FILE] = CNTS_users_footer_file,
  [CONTEST_REGISTER_EMAIL] = CNTS_register_email,
  [CONTEST_REGISTER_URL] = CNTS_register_url,
  [CONTEST_LOGIN_TEMPLATE] = CNTS_login_template,
  [CONTEST_TEAM_URL] = CNTS_team_url,
  [CONTEST_REGISTRATION_DEADLINE] = CNTS_reg_deadline,
  [CONTEST_START_DATE] = CNTS_start_date,
  [CONTEST_ROOT_DIR] = CNTS_root_dir,
  [CONTEST_STANDINGS_URL] = CNTS_standings_url,
  [CONTEST_PROBLEMS_URL] = CNTS_problems_url,
  [CONTEST_SERVE_USER] = CNTS_serve_user,
  [CONTEST_SERVE_GROUP] = CNTS_serve_group,
  [CONTEST_REGISTER_HEADER_FILE] = CNTS_register_header_file,
  [CONTEST_REGISTER_FOOTER_FILE] = CNTS_register_footer_file,
  [CONTEST_TEAM_HEADER_FILE] = CNTS_team_header_file,
  [CONTEST_TEAM_MENU_1_FILE] = CNTS_team_menu_1_file,
  [CONTEST_TEAM_MENU_2_FILE] = CNTS_team_menu_2_file,
  [CONTEST_TEAM_MENU_3_FILE] = CNTS_team_menu_3_file,
  [CONTEST_TEAM_SEPARATOR_FILE] = CNTS_team_separator_file,
  [CONTEST_TEAM_FOOTER_FILE] = CNTS_team_footer_file,
  [CONTEST_COPYRIGHT_FILE] = CNTS_copyright_file,
  [CONTEST_USERS_HEAD_STYLE] = CNTS_users_head_style,
  [CONTEST_USERS_PAR_STYLE] = CNTS_users_par_style,
  [CONTEST_USERS_TABLE_STYLE] = CNTS_users_table_style,
  [CONTEST_USERS_VERB_STYLE] = CNTS_users_verb_style,
  [CONTEST_USERS_TABLE_FORMAT] = CNTS_users_table_format,
  [CONTEST_USERS_TABLE_FORMAT_EN] = CNTS_users_table_format_en,
  [CONTEST_USERS_TABLE_LEGEND] = CNTS_users_table_legend,
  [CONTEST_USERS_TABLE_LEGEND_EN] = CNTS_users_table_legend_en,
  [CONTEST_REGISTER_HEAD_STYLE] = CNTS_register_head_style,
  [CONTEST_REGISTER_PAR_STYLE] = CNTS_register_par_style,
  [CONTEST_REGISTER_TABLE_STYLE] = CNTS_register_table_style,
  [CONTEST_TEAM_HEAD_STYLE] = CNTS_team_head_style,
  [CONTEST_TEAM_PAR_STYLE] = CNTS_team_par_style,
  [CONTEST_CONF_DIR] = CNTS_conf_dir,
  [CONTEST_RUN_USER] = CNTS_run_user,
  [CONTEST_RUN_GROUP] = CNTS_run_group,
  [CONTEST_REGISTER_EMAIL_FILE] = CNTS_register_email_file,
  [CONTEST_USER_NAME_COMMENT] = CNTS_user_name_comment,
  [CONTEST_ALLOWED_LANGUAGES] = CNTS_allowed_languages,
  [CONTEST_CF_NOTIFY_EMAIL] = CNTS_cf_notify_email,
  [CONTEST_CLAR_NOTIFY_EMAIL] = CNTS_clar_notify_email,
  [CONTEST_DAILY_STAT_EMAIL] = CNTS_daily_stat_email,
  [CONTEST_PRIV_HEADER_FILE] = CNTS_priv_header_file,
  [CONTEST_PRIV_FOOTER_FILE] = CNTS_priv_footer_file,
  [CONTEST_ALLOWED_REGIONS] = CNTS_allowed_regions,
  [CONTEST_LOGIN_TEMPLATE_OPTIONS] = CNTS_login_template_options,
  [CONTEST_DIR_MODE] = CNTS_dir_mode,
  [CONTEST_DIR_GROUP] = CNTS_dir_group,
  [CONTEST_FILE_MODE] = CNTS_file_mode,
  [CONTEST_FILE_GROUP] = CNTS_file_group,
  [CONTEST_DEFAULT_LOCALE] = CNTS_default_locale,
  [CONTEST_WELCOME_FILE] = CNTS_welcome_file,
  [CONTEST_REG_WELCOME_FILE] = CNTS_reg_welcome_file,
  [CONTEST_USER_CONTEST] = CNTS_user_contest,
};
static const int attr_to_contest_desc_map[CONTEST_LAST_ATTR] =
{
  [CONTEST_A_ID] = CNTS_id,
  [CONTEST_A_AUTOREGISTER] = CNTS_autoregister,
  [CONTEST_A_DISABLE_TEAM_PASSWORD] = CNTS_disable_team_password,
  [CONTEST_A_MANAGED] = CNTS_managed,
  [CONTEST_A_NEW_MANAGED] = CNTS_managed,
  [CONTEST_A_CLEAN_USERS] = CNTS_clean_users,
  [CONTEST_A_RUN_MANAGED] = CNTS_run_managed,
  [CONTEST_A_CLOSED] = CNTS_closed,
  [CONTEST_A_INVISIBLE] = CNTS_invisible,
  [CONTEST_A_SIMPLE_REGISTRATION] = CNTS_simple_registration,
  [CONTEST_A_SEND_PASSWD_EMAIL] = CNTS_send_passwd_email,
  [CONTEST_A_ASSIGN_LOGINS] = CNTS_assign_logins,
  [CONTEST_A_FORCE_REGISTRATION] = CNTS_force_registration,
  [CONTEST_A_DISABLE_NAME] = CNTS_disable_name,
  [CONTEST_A_ENABLE_FORGOT_PASSWORD] = CNTS_enable_password_recovery,
  [CONTEST_A_EXAM_MODE] = CNTS_exam_mode,
  [CONTEST_A_DISABLE_PASSWORD_CHANGE] = CNTS_disable_password_change,
  [CONTEST_A_DISABLE_LOCALE_CHANGE] = CNTS_disable_locale_change,
  [CONTEST_A_PERSONAL] = CNTS_personal,
  [CONTEST_A_ALLOW_REG_DATA_EDIT] = CNTS_allow_reg_data_edit,
  [CONTEST_A_ENABLE_PASSWORD_RECOVERY] = CNTS_enable_password_recovery,
  [CONTEST_A_DISABLE_MEMBER_DELETE] = CNTS_disable_member_delete,
};

static char const * const elem_map[] =
{
  0,
  "contests",
  "contest",
  "register_access",
  "users_access",
  "master_access",
  "judge_access",
  "team_access",
  "serve_control_access",
  "ip",
  "field",
  "name",
  "name_en",
  "main_url",
  "keywords",
  "contestants",
  "reserves",
  "coaches",
  "advisors",
  "guests",
  "users_header_file",
  "users_footer_file",
  "register_email",
  "register_url",
  "login_template",
  "team_url",
  "registration_deadline",
  "start_date",
  "cap",
  "caps",
  "root_dir",
  "standings_url",
  "problems_url",
  "client_flags",
  "serve_user",
  "serve_group",
  "register_header_file",
  "register_footer_file",
  "team_header_file",
  "team_menu_1_file",
  "team_menu_2_file",
  "team_menu_3_file",
  "team_separator_file",
  "team_footer_file",
  "copyright_file",
  "users_head_style",
  "users_par_style",
  "users_table_style",
  "users_verb_style",
  "users_table_format",
  "users_table_format_en",
  "users_table_legend",
  "users_table_legend_en",
  "register_head_style",
  "register_par_style",
  "register_table_style",
  "team_head_style",
  "team_par_style",
  "conf_dir",
  "run_user",
  "run_group",
  "register_email_file",
  "user_name_comment",
  "allowed_languages",
  "cf_notify_email",
  "clar_notify_email",
  "daily_stat_email",
  "priv_header_file",
  "priv_footer_file",
  "allowed_regions",
  "login_template_options",
  "dir_mode",
  "dir_group",
  "file_mode",
  "file_group",
  "default_locale",
  "welcome_file",
  "reg_welcome_file",
  "slave_rules",
  "run_managed_on",
  "user_contest",

  0
};
static char const * const attr_map[] =
{
  0,
  "id",
  "default",
  "allow",
  "deny",
  "mandatory",
  "optional",
  "min",
  "max",
  "autoregister",
  "initial",
  "disable_team_password",
  "login",
  "managed",
  "new_managed",
  "clean_users",
  "run_managed",
  "closed",
  "invisible",
  "ssl",
  "simple_registration",
  "send_passwd_email",
  "assign_logins",
  "force_registration",
  "disable_name",
  "enable_forgot_password",
  "exam_mode",
  "disable_password_change",
  "disable_locale_change",
  "personal",
  "allow_reg_data_edit",
  "enable_password_recovery",
  "disable_member_delete",

  0
};
static size_t const elem_sizes[CONTEST_LAST_TAG] =
{
  [CONTEST_CONTESTS] = sizeof(struct contest_list),
  [CONTEST_CONTEST] = sizeof(struct contest_desc),
  [CONTEST_REGISTER_ACCESS] = sizeof(struct contest_access),
  [CONTEST_USERS_ACCESS] = sizeof(struct contest_access),
  [CONTEST_MASTER_ACCESS] = sizeof(struct contest_access),
  [CONTEST_JUDGE_ACCESS] = sizeof(struct contest_access),
  [CONTEST_TEAM_ACCESS] = sizeof(struct contest_access),
  [CONTEST_SERVE_CONTROL_ACCESS] = sizeof(struct contest_access),
  [CONTEST_IP] = sizeof(struct contest_ip),
  [CONTEST_FIELD] = sizeof(struct contest_field),
  [CONTEST_CONTESTANTS] = sizeof(struct contest_member),
  [CONTEST_RESERVES] = sizeof(struct contest_member),
  [CONTEST_COACHES] = sizeof(struct contest_member),
  [CONTEST_ADVISORS] = sizeof(struct contest_member),
  [CONTEST_GUESTS] = sizeof(struct contest_member),
  [CONTEST_CAP] = sizeof(struct opcap_list_item),
};

static void
node_free(struct xml_tree *t)
{
  int i;

  switch (t->tag) {
  case CONTEST_CONTESTS:
    xfree(((struct contest_list *) t)->id_map);
    break;
  case CONTEST_CONTEST:
    {
      struct contest_desc *cnts = (struct contest_desc*) t;
      // free everything of type 's'
      for (i = 1; i < CNTS_LAST_FIELD; ++i) {
        if (contest_desc_get_type(i) != 's') continue;
        unsigned char **p = (unsigned char **) contest_desc_get_ptr_nc(cnts, i);
        xfree(*p); *p = 0;
      }
    }
    break;
  case CONTEST_CAP:
    {
      struct opcap_list_item *pp = (struct opcap_list_item*) t;
      xfree(pp->login);
    }
    break;
  case CONTEST_FIELD:
    {
      struct contest_field *ff = (struct contest_field*) t;
      xfree(ff->legend);
    }
    break;
  }
}

static struct xml_parse_spec contests_parse_spec =
{
  .elem_map = elem_map,
  .attr_map = attr_map,
  .elem_sizes = elem_sizes,
  .attr_sizes = NULL,
  .default_elem = 0,
  .default_attr = 0,
  .elem_alloc = NULL,
  .attr_alloc = NULL,
  .elem_free = node_free,
  .attr_free = NULL,
};

static char const * const field_map[] =
{
  0,
  "homepage",
  "phone",
  "inst",
  "inst_en",
  "instshort",
  "instshort_en",
  "instnum",
  "fac",
  "fac_en",
  "facshort",
  "facshort_en",
  "city",
  "city_en",
  "country",
  "country_en",
  "region",
  "area",
  "zip",
  "street",
  "languages",
  "field0",
  "field1",
  "field2",
  "field3",
  "field4",
  "field5",
  "field6",
  "field7",
  "field8",
  "field9",

  0
};

static char const * const member_field_map[] =
{
  0,
  "firstname",
  "firstname_en",
  "middlename",
  "middlename_en",
  "surname",
  "surname_en",
  "status",
  "gender",
  "grade",
  "group",
  "group_en",
  "email",
  "homepage",
  "phone",
  "inst",
  "inst_en",
  "instshort",
  "instshort_en",
  "fac",
  "fac_en",
  "facshort",
  "facshort_en",
  "occupation",
  "occupation_en",
  "discipline",
  "birth_date",
  "entry_date",
  "graduation_date",

  0,
};

static int
parse_access(struct contest_access *acc, char const *path)
{
  struct xml_attr *a;
  struct xml_tree *t;
  struct contest_ip *ip;

  for (a = acc->b.first; a; a = a->next) {
    switch (a->tag) {
    case CONTEST_A_DEFAULT:
      if (!strcasecmp(a->text, "allow")) {
        acc->default_is_allow = 1;
      } else if (!strcasecmp(a->text, "deny")) {
        acc->default_is_allow = 0;
      } else {
        return xml_err_attr_invalid(a);
      }
      xfree(a->text); a->text = 0;
      break;
    default:
      return xml_err_attr_not_allowed(&acc->b, a);
    }
  }

  for (t = acc->b.first_down; t; t = t->right) {
    if (t->tag != CONTEST_IP) return xml_err_elem_not_allowed(t);
    if (t->first_down) return xml_err_nested_elems(t);

    ip = (struct contest_ip*) t;
    ip->allow = -1;
    ip->ssl = -1;
    for (a = ip->b.first; a; a = a->next) {
      if (a->tag == CONTEST_A_SSL) {
        if (!strcasecmp(a->text, "yes")) {
          ip->ssl = 1;
        } else if (!strcasecmp(a->text, "no")) {
          ip->ssl = 0;
        } else if (!strcasecmp(a->text, "any")) {
          ip->ssl = -1;
        } else {
          return xml_err_attr_invalid(a);
        }
        xfree(a->text); a->text = 0;
        continue;
      }
      if (a->tag != CONTEST_A_ALLOW && a->tag != CONTEST_A_DENY)
        return xml_err_attr_not_allowed(&ip->b, a);
      if (ip->allow != -1) {
        xml_err_a(a, "attribute \"allow\" already defined");
        return -1;
      }
      if (xml_attr_bool(a, &ip->allow) < 0) return -1;
      if (a->tag == CONTEST_A_DENY) ip->allow = !ip->allow;
      xfree(a->text); a->text = 0;
    }
    if (ip->allow == -1) ip->allow = 0;

    if (xml_parse_ip_mask(path, ip->b.line, ip->b.column,
                          ip->b.text, &ip->addr, &ip->mask) < 0) return -1;
    xfree(t->text); t->text = 0;
  }

  xfree(acc->b.text); acc->b.text = 0;
  return 0;
}

static int
parse_member(struct contest_member *mb, char const *path)
{
  struct xml_attr *a;
  struct xml_tree *t;
  struct contest_field *pf;
  int i, n;

  /*
  mb->min_count = -1;
  mb->max_count = -1;
  */
  for (a = mb->b.first; a; a = a->next) {
    switch (a->tag) {
    case CONTEST_A_MIN:
    case CONTEST_A_MAX:
    case CONTEST_A_INITIAL:
      if (!a->text || sscanf(a->text, "%d %n", &i, &n) != 1
          || a->text[n] || i < 0 || i > 100)
        return xml_err_attr_invalid(a);
      switch (a->tag) {
      case CONTEST_A_MIN:     mb->min_count = i;  break;
      case CONTEST_A_MAX:     mb->max_count = i;  break;
      case CONTEST_A_INITIAL: mb->init_count = i; break;
      }
      xfree(a->text); a->text = 0;
      break;
    default:
      return xml_err_attr_not_allowed(&mb->b, a);
    }
  }

  xfree(mb->b.text); mb->b.text = 0;
  for (t = mb->b.first_down; t; t = t->right) {
    if (t->tag != CONTEST_FIELD) return xml_err_elem_not_allowed(t);
    if (t->first_down) return xml_err_nested_elems(t);
    pf = (struct contest_field*) t;
    if (t->text && *t->text) {
      pf->legend = t->text; t->text = 0;
    }
    xfree(t->text); t->text = 0;

    pf->mandatory = -1;
    for (a = t->first; a; a = a->next) {
      switch (a->tag) {
      case CONTEST_A_ID:
        for (i = 1; i < CONTEST_LAST_MEMBER_FIELD; i++) {
          if (!member_field_map[i]) continue;
          if (!strcmp(a->text, member_field_map[i])) break;
        }
        if (i >= CONTEST_LAST_MEMBER_FIELD) {
          xml_err_a(a, "invalid field id \"%s\"", a->text);
          return -1;
        }
        if (mb->fields[i]) {
          xml_err_a(a, "field \"%s\" already defined", a->text);
          return -1;
        }
        mb->fields[i] = pf;
        break;
      case CONTEST_A_MANDATORY:
      case CONTEST_A_OPTIONAL:
        if (pf->mandatory != -1) {
          xml_err_a(a, "attribute \"mandatory\" already defined");
          return -1;
        }
        if (xml_attr_bool(a, &pf->mandatory) < 0) return -1;
        if (a->tag == CONTEST_A_OPTIONAL) pf->mandatory = !pf->mandatory;
        break;
      default:
        return xml_err_attr_not_allowed(t, a);
      }
    }
    if (pf->mandatory == -1) pf->mandatory = 0;
  }
  return 0;
}

static int
handle_final_tag(char const *path, struct xml_tree *t, unsigned char **ps)
{
  if (*ps) {
    err("%s:%d:%d: duplicated element <%s>",
        path, t->line, t->column, elem_map[t->tag]);
    return -1;
  }
  if (!t->text || !*t->text) {
    err("%s:%d:%d: empty element <%s>", path, t->line, t->column,
        elem_map[t->tag]);
    return -1;
  }
  if (t->first_down) {
    err("%s:%d:%d: element <%s> cannot contain nested elements",
        path, t->line, t->column, elem_map[t->tag]);
    return -1;
  }
  if (t->first) {
    err("%s:%d:%d: element <%s> cannot have attributes",
        path, t->line, t->column, elem_map[t->tag]);
    return -1;
  }
  *ps = t->text; t->text = 0;
  return 0;
}

static int
parse_capabilities(unsigned char const *path,
                   struct contest_desc *cnts,
                   struct xml_tree *ct)
{
  struct xml_tree *p;
  struct opcap_list_item *pp, *qq;

  ASSERT(ct->tag == CONTEST_CAPS);

  if (cnts->capabilities.first) return xml_err_elem_redefined(ct);

  cnts->caps_node = ct;
  xfree(ct->text); ct->text = 0;
  if (ct->first) return xml_err_attrs(ct);
  p = ct->first_down;
  if (!p) return 0;
  cnts->capabilities.first = (struct opcap_list_item*) p;

  for (; p; p = p->right) {
    if (p->tag != CONTEST_CAP) return xml_err_elem_not_allowed(p);
    pp = (struct opcap_list_item*) p;

    if (!p->first) return xml_err_attr_undefined(p, CONTEST_A_LOGIN);
    if (p->first->next) return xml_err_attr_not_allowed(p, p->first->next);
    if (p->first->tag != CONTEST_A_LOGIN)
      return xml_err_attr_undefined(p, CONTEST_A_LOGIN);
    pp->login = p->first->text; p->first->text = 0;
    if (!pp->login || !*pp->login) return xml_err_attr_invalid(p->first);
    for (qq = CNTS_FIRST_PERM(cnts); qq != pp; qq = CNTS_NEXT_PERM_NC(qq)) {
      if (!strcmp(pp->login, qq->login)) {
        xml_err(p, "duplicated login");
        return -1;
      }
    }
    if (opcaps_parse(p->text, &pp->caps) < 0) return xml_err_elem_invalid(p);
    xfree(p->text); p->text = 0;
  }
  return 0;
}

static int
parse_client_flags(unsigned char const *path, struct contest_desc *cnts,
                   struct xml_tree *xt)
{
  int len;
  unsigned char *str2, *q, *str3;
  unsigned char const *p, *s, *str;

  str = xt->text;
  if (!str) str = "";
  len = strlen(str);
  str2 = (unsigned char *) alloca(len + 10);
  for (p = str, q = str2; *p; p++) {
    if (isspace(*p)) continue;
    if (isalpha(*p)) {
      *q++ = toupper(*p);
    } else {
      *q++ = *p;
    }
  }
  *q++ = 0;

  str3 = (unsigned char *) alloca(len + 10);
  p = str2;
  while (1) {
    while (*p == ',') p++;
    if (!*p) break;
    for (s = p; *s && *s != ','; s++);
    memset(str3, 0, len + 10);
    memcpy(str3, p, s - p);
    p = s;

    if (!strcmp(str3, "IGNORE_TIME_SKEW")) {
      /* DO NOTHING: compatibility with previous versions */
    } else if (!strcmp(str3, "DISABLE_TEAM")) {
      /* DO NOTHING: compatibility with previous versions */
    } else if (!strcmp(str3, "DISABLE_MEMBER_DELETE")) {
      cnts->disable_member_delete = 1;
    } else {
      return xml_err_elem_invalid(xt);
    }
  }

  xfree(xt->text); xt->text = 0;
  return 0;
}

static void
process_conf_file_path(struct contest_desc *cnts, unsigned char **pstr)
{
  unsigned char *str = *pstr;
  unsigned char pathbuf[PATH_MAX];

  if (!str || os_IsAbsolutePath(str) || !cnts->conf_dir) return;
  snprintf(pathbuf, sizeof(pathbuf), "%s/%s", cnts->conf_dir, str);
  xfree(str);
  str = xstrdup(pathbuf);
  *pstr = str;
}

static const unsigned char contest_final_set[CONTEST_LAST_TAG] =
{
  [CONTEST_NAME] = 1,
  [CONTEST_NAME_EN] = 1,
  [CONTEST_MAIN_URL] = 1,
  [CONTEST_KEYWORDS] = 1,
  [CONTEST_USERS_HEADER_FILE] = 1,
  [CONTEST_USERS_FOOTER_FILE] = 1,
  [CONTEST_REGISTER_EMAIL] = 1,
  [CONTEST_REGISTER_URL] = 1,
  [CONTEST_LOGIN_TEMPLATE] = 1,
  [CONTEST_LOGIN_TEMPLATE_OPTIONS] = 1,
  [CONTEST_TEAM_URL] = 1,
  [CONTEST_ROOT_DIR] = 1,
  [CONTEST_STANDINGS_URL] = 1,
  [CONTEST_PROBLEMS_URL] = 1,
  [CONTEST_SERVE_USER] = 1,
  [CONTEST_SERVE_GROUP] = 1,
  [CONTEST_REGISTER_HEADER_FILE] = 1,
  [CONTEST_REGISTER_FOOTER_FILE] = 1,
  [CONTEST_TEAM_HEADER_FILE] = 1,
  [CONTEST_TEAM_MENU_1_FILE] = 1,
  [CONTEST_TEAM_MENU_2_FILE] = 1,
  [CONTEST_TEAM_MENU_3_FILE] = 1,
  [CONTEST_TEAM_SEPARATOR_FILE] = 1,
  [CONTEST_TEAM_FOOTER_FILE] = 1,
  [CONTEST_COPYRIGHT_FILE] = 1,
  [CONTEST_USERS_HEAD_STYLE] = 1,
  [CONTEST_USERS_PAR_STYLE] = 1,
  [CONTEST_USERS_TABLE_STYLE] = 1,
  [CONTEST_USERS_VERB_STYLE] = 1,
  [CONTEST_USERS_TABLE_FORMAT] = 1,
  [CONTEST_USERS_TABLE_FORMAT_EN] = 1,
  [CONTEST_USERS_TABLE_LEGEND] = 1,
  [CONTEST_USERS_TABLE_LEGEND_EN] = 1,
  [CONTEST_REGISTER_HEAD_STYLE] = 1,
  [CONTEST_REGISTER_PAR_STYLE] = 1,
  [CONTEST_REGISTER_TABLE_STYLE] = 1,
  [CONTEST_TEAM_HEAD_STYLE] = 1,
  [CONTEST_TEAM_PAR_STYLE] = 1,
  [CONTEST_CONF_DIR] = 1,
  [CONTEST_RUN_USER] = 1,
  [CONTEST_RUN_GROUP] = 1,
  [CONTEST_REGISTER_EMAIL_FILE] = 1,
  [CONTEST_USER_NAME_COMMENT] = 1,
  [CONTEST_ALLOWED_LANGUAGES] = 1,
  [CONTEST_ALLOWED_REGIONS] = 1,
  [CONTEST_CF_NOTIFY_EMAIL] = 1,
  [CONTEST_CLAR_NOTIFY_EMAIL] = 1,
  [CONTEST_DAILY_STAT_EMAIL] = 1,
  [CONTEST_PRIV_HEADER_FILE] = 1,
  [CONTEST_PRIV_FOOTER_FILE] = 1,
  [CONTEST_DIR_MODE] = 1,
  [CONTEST_DIR_GROUP] = 1,
  [CONTEST_FILE_MODE] = 1,
  [CONTEST_FILE_GROUP] = 1,
  [CONTEST_DEFAULT_LOCALE] = 1,
  [CONTEST_WELCOME_FILE] = 1,
  [CONTEST_REG_WELCOME_FILE] = 1,
  [CONTEST_USER_CONTEST] = 1,
};

static const unsigned char contest_access_set[CONTEST_LAST_TAG] =
{
  [CONTEST_REGISTER_ACCESS] = 1,
  [CONTEST_USERS_ACCESS] = 1,
  [CONTEST_MASTER_ACCESS] = 1,
  [CONTEST_JUDGE_ACCESS] = 1,
  [CONTEST_TEAM_ACCESS] = 1,
  [CONTEST_SERVE_CONTROL_ACCESS] = 1,
};

static const unsigned char contest_bool_attr_set[CONTEST_LAST_ATTR] =
{
  [CONTEST_A_AUTOREGISTER] = 1,
  [CONTEST_A_DISABLE_TEAM_PASSWORD] = 1,
  [CONTEST_A_MANAGED] = 1,
  [CONTEST_A_NEW_MANAGED] = 1,
  [CONTEST_A_CLEAN_USERS] = 1,
  [CONTEST_A_RUN_MANAGED] = 1,
  [CONTEST_A_CLOSED] = 1,
  [CONTEST_A_INVISIBLE] = 1,
  [CONTEST_A_SIMPLE_REGISTRATION] = 1,
  [CONTEST_A_SEND_PASSWD_EMAIL] = 1,
  [CONTEST_A_ASSIGN_LOGINS] = 1,
  [CONTEST_A_FORCE_REGISTRATION] = 1,
  [CONTEST_A_DISABLE_NAME] = 1,
  [CONTEST_A_ENABLE_FORGOT_PASSWORD] = 1,
  [CONTEST_A_EXAM_MODE] = 1,
  [CONTEST_A_DISABLE_PASSWORD_CHANGE] = 1,
  [CONTEST_A_DISABLE_LOCALE_CHANGE] = 1,
  [CONTEST_A_PERSONAL] = 1,
  [CONTEST_A_ALLOW_REG_DATA_EDIT] = 1,
  [CONTEST_A_ENABLE_PASSWORD_RECOVERY] = 1,
  [CONTEST_A_DISABLE_MEMBER_DELETE] = 1,
};

static void
fix_personal_contest(struct contest_desc *cnts)
{
  struct xml_tree *p;
  struct contest_member *m;

  if (!cnts->personal) return;

  if (!cnts->members[CONTEST_M_CONTESTANT]) {
    p = contests_new_node(CONTEST_CONTESTANTS);
    xml_link_node_last(&cnts->b, p);
    cnts->members[CONTEST_M_CONTESTANT] = (struct contest_member*) p;
  }
  m = cnts->members[CONTEST_M_CONTESTANT];
  m->min_count = 1;
  m->max_count = 1;
  m->init_count = 1;

  if (cnts->members[CONTEST_M_RESERVE]) {
    p = (struct xml_tree*) cnts->members[CONTEST_M_RESERVE];
    xml_unlink_node(p);
    xml_tree_free(p, &contests_parse_spec);
    cnts->members[CONTEST_M_RESERVE] = 0;
  }
}

static int
parse_contest(struct contest_desc *cnts, char const *path, int no_subst_flag)
{
  struct xml_attr *a;
  struct xml_tree *t;
  int x, n, mb_id, i;
  unsigned char *date_str = 0;
  struct contest_access **pacc;
  unsigned char pathbuf[PATH_MAX];
  unsigned char *p_field;
  unsigned char **p_str;
  char *eptr;
  const int *flist;

  cnts->clean_users = 1;

  for (a = cnts->b.first; a; a = a->next) {
    if (contest_bool_attr_set[a->tag] > 0) {
      // boolean fields
      p_field = (unsigned char*) contest_desc_get_ptr_nc(cnts, attr_to_contest_desc_map[a->tag]);
      if (xml_attr_bool_byte(a, p_field) < 0) return -1;
      continue;
    }

    switch (a->tag) {
    case CONTEST_A_ID:
      x = n = 0;
      if (sscanf(a->text, "%d %n", &x, &n) != 1 || a->text[n]
          || x <= 0 || x > EJ_MAX_CONTEST_ID) return xml_err_attr_invalid(a);
      cnts->id = x;
      break;
    default:
      return xml_err_attr_not_allowed(&cnts->b, a);
    }
  }

  if (!cnts->id) return xml_err_attr_undefined(&cnts->b, CONTEST_A_ID);

  for (t = cnts->b.first_down; t; t = t->right) {
    if (contest_final_set[t->tag] > 0) {
      p_str = (unsigned char**) contest_desc_get_ptr_nc(cnts, tag_to_contest_desc_map[t->tag]);
      if (xml_leaf_elem(t, p_str, 1, 0) < 0) return -1;
      continue;
    }
    if (contest_access_set[t->tag] > 0) {
      pacc = (struct contest_access**) contest_desc_get_ptr_nc(cnts, tag_to_contest_desc_map[t->tag]);
      if (*pacc) return xml_err_elem_redefined(t);
      *pacc = (struct contest_access*) t;
      if (parse_access(*pacc, path) < 0) return -1;
      continue;
    }

    switch(t->tag) {
    case CONTEST_CLIENT_FLAGS:
      if (t->first_down) return xml_err_nested_elems(t);
      if (t->first) return xml_err_attrs(t);
      if (parse_client_flags(path, cnts, t) < 0) return -1;
      break;
    case CONTEST_REGISTRATION_DEADLINE:
    case CONTEST_START_DATE:
      if (handle_final_tag(path, t, &date_str) < 0) {
        xfree(date_str);
        return -1;
      }
      if (xml_parse_date(path, t->line, t->column, date_str, (time_t*) contest_desc_get_ptr_nc(cnts, tag_to_contest_desc_map[t->tag])) < 0) {
        xfree(date_str); date_str = 0;
        return -1;
      }
      xfree(date_str); date_str = 0;
      break;
    case CONTEST_SLAVE_RULES:
      cnts->slave_rules = t;
      break;

    case CONTEST_CAPS:
      if (parse_capabilities(path, cnts, t) < 0) return -1;
      break;

    case CONTEST_CONTESTANTS:
      mb_id = CONTEST_M_CONTESTANT;
      goto process_members;
    case CONTEST_RESERVES:
      mb_id = CONTEST_M_RESERVE;
      goto process_members;
    case CONTEST_COACHES:
      mb_id = CONTEST_M_COACH;
      goto process_members;
    case CONTEST_ADVISORS:
      mb_id = CONTEST_M_ADVISOR;
      goto process_members;
    case CONTEST_GUESTS:
      mb_id = CONTEST_M_GUEST;

    process_members:
      if (cnts->members[mb_id]) return xml_err_elem_redefined(t);
      if (parse_member((struct contest_member*) t, path) < 0)
        return -1;
      cnts->members[mb_id] = (struct contest_member*) t;
      break;

    case CONTEST_FIELD:
      if (t->first_down) return xml_err_nested_elems(t);
      {
        struct contest_field *pf = (struct contest_field*) t;
        int i;

        if (t->text && *t->text) {
          pf->legend = t->text; t->text = 0;
        }
        xfree(t->text); t->text = 0;

        pf->mandatory = -1;
        for (a = t->first; a; a = a->next) {
          switch (a->tag) {
          case CONTEST_A_ID:
            for (i = 1; i < CONTEST_LAST_FIELD; i++) {
              if (!field_map[i]) continue;
              if (!strcmp(a->text, field_map[i])) break;
            }
            if (i >= CONTEST_LAST_FIELD) {
              xml_err_a(a, "invalid field id \"%s\"", a->text);
              return -1;
            }
            if (cnts->fields[i]) {
              xml_err_a(a, "field \"%s\" already defined", a->text);
              return -1;
            }
            cnts->fields[i] = pf;
            break;
          case CONTEST_A_MANDATORY:
          case CONTEST_A_OPTIONAL:
            if (pf->mandatory != -1) {
              xml_err_a(a, "attribute \"mandatory\" already defined");
              return -1;
            }
            if (xml_attr_bool(a, &pf->mandatory) < 0) return -1;
            if (a->tag == CONTEST_A_OPTIONAL) pf->mandatory = !pf->mandatory;
            break;
          default:
            return xml_err_attr_not_allowed(t, a);
          }
        }
        if (pf->mandatory == -1) pf->mandatory = 0;
      }
      break;

    default:
      return xml_err_elem_not_allowed(t);
    }
  }
  xfree(cnts->b.text); cnts->b.text = 0;

  if (!cnts->name) return xml_err_elem_undefined(&cnts->b, CONTEST_NAME);

  if (!cnts->root_dir) {
    // use the standard pattern?
    snprintf(pathbuf, sizeof(pathbuf), "%06d", cnts->id);
    cnts->root_dir = xstrdup(pathbuf);
  }
  if (!os_IsAbsolutePath(cnts->root_dir) && ejudge_config
      && ejudge_config->contests_home_dir
      && os_IsAbsolutePath(ejudge_config->contests_home_dir)) {
    snprintf(pathbuf, sizeof(pathbuf), "%s/%s",
             ejudge_config->contests_home_dir, cnts->root_dir);
    xfree(cnts->root_dir);
    cnts->root_dir = xstrdup(pathbuf);
  }
#if defined EJUDGE_CONTESTS_HOME_DIR
  if (!os_IsAbsolutePath(cnts->root_dir)
      && os_IsAbsolutePath(EJUDGE_CONTESTS_HOME_DIR)) {
    snprintf(pathbuf, sizeof(pathbuf), "%s/%s", EJUDGE_CONTESTS_HOME_DIR,
             cnts->root_dir);
    xfree(cnts->root_dir);
    cnts->root_dir = xstrdup(pathbuf);
  }
#endif
  if (!os_IsAbsolutePath(cnts->root_dir)) {
    xml_err(&cnts->b, "<root_dir> must be absolute path");
    return -1;
  }

  if (!no_subst_flag) {
    if (cnts->root_dir && !cnts->conf_dir) {
      snprintf(pathbuf, sizeof(pathbuf), "%s/conf", cnts->root_dir);
      cnts->conf_dir = xstrdup(pathbuf);
    } else if (cnts->root_dir && !os_IsAbsolutePath(cnts->conf_dir)) {
      snprintf(pathbuf, sizeof(pathbuf), "%s/%s", cnts->root_dir,cnts->conf_dir);
      xfree(cnts->conf_dir);
      cnts->conf_dir = xstrdup(pathbuf);
    } else if (!cnts->root_dir && cnts->conf_dir
               && !os_IsAbsolutePath(cnts->conf_dir)) {
      xml_err(&cnts->b, "<conf_dir> must be absolute path");
      return -1;
    }

    flist = (const int[]) {
      CNTS_register_header_file, CNTS_register_footer_file,
      CNTS_users_header_file, CNTS_users_footer_file,
      CNTS_team_header_file, CNTS_team_menu_1_file, CNTS_team_menu_2_file,
      CNTS_team_menu_3_file, CNTS_team_separator_file, CNTS_team_footer_file,
      CNTS_copyright_file, CNTS_register_email_file,
      CNTS_priv_header_file, CNTS_priv_footer_file,
      CNTS_welcome_file, CNTS_reg_welcome_file,
      0 };
    for (i = 0; flist[i]; ++i) {
      process_conf_file_path(cnts,
                             (unsigned char**)contest_desc_get_ptr_nc(cnts, flist[i]));
    }

    if (!cnts->users_head_style) {
      cnts->users_head_style = xstrdup("h2");
    }
    if (!cnts->register_head_style) {
      cnts->register_head_style = xstrdup("h2");
    }
    if (!cnts->team_head_style) {
      cnts->team_head_style = xstrdup("h2");
    }
    if (!cnts->users_par_style)
      cnts->users_par_style = xstrdup("");
    if (!cnts->register_par_style)
      cnts->register_par_style = xstrdup("");
    if (!cnts->team_par_style)
      cnts->team_par_style = xstrdup("");
    if (!cnts->users_table_style)
      cnts->users_table_style = xstrdup("");
    if (!cnts->register_table_style)
      cnts->register_table_style = xstrdup("");
    if (!cnts->users_verb_style)
      cnts->users_verb_style = xstrdup("");
  }

  if (cnts->user_contest) {
    errno = 0;
    cnts->user_contest_num = strtol(cnts->user_contest, &eptr, 10);
    if (*eptr || errno || cnts->user_contest_num < 0) {
      xml_err(&cnts->b, "invalid value of <user_contest>");
      return -1;
    }
  }

  /* personal contests do not have "reserve" and have only one participant */
  fix_personal_contest(cnts);

  cnts->default_locale_num = l10n_parse_locale(cnts->default_locale);

  return 0;
}

static struct contest_desc *
parse_one_contest_xml(char const *path, int number, int no_subst_flag)
{
  struct xml_tree *tree = 0;
  struct contest_desc *d = 0;

  xml_err_path = path;
  xml_err_spec = &contests_parse_spec;

  tree = xml_build_tree(path, &contests_parse_spec);
  if (!tree) goto failed;
  if (tree->tag != CONTEST_CONTEST) {
    xml_err_top_level(tree, CONTEST_CONTEST);
    goto failed;
  }
  d = (struct contest_desc *) tree;
  if (parse_contest(d, path, no_subst_flag) < 0) goto failed;
  return d;

 failed:
  if (tree) xml_tree_free(tree, &contests_parse_spec);
  return 0;
}

static void
contests_merge(struct contest_desc *pold, struct contest_desc *pnew)
{
  struct xml_tree *p, *q;
  int i;
  unsigned char **p_str_old, **p_str_new;
  struct contest_access **p_acc_old, **p_acc_new;
  unsigned char *p_b_old, *p_b_new;

  // unlink and free all the old root node childs
  for (p = pold->b.first_down; p; p = q) {
    q = p->right;
    xml_unlink_node(p);
    xml_tree_free(p, &contests_parse_spec);
  }

  // copy offsetted fields
  for (i = 0; i < CONTEST_LAST_TAG; i++) {
    if (contest_final_set[i]) {
      p_str_old = (unsigned char**) contest_desc_get_ptr_nc(pold, tag_to_contest_desc_map[i]);
      p_str_new = (unsigned char**) contest_desc_get_ptr_nc(pnew, tag_to_contest_desc_map[i]);
      xfree(*p_str_old);
      *p_str_old = *p_str_new;
      *p_str_new = 0;
    } else if (contest_access_set[i]) {
      p_acc_old = (struct contest_access**) contest_desc_get_ptr_nc(pold, tag_to_contest_desc_map[i]);
      p_acc_new = (struct contest_access**) contest_desc_get_ptr_nc(pnew, tag_to_contest_desc_map[i]);
      p = &(*p_acc_new)->b;
      if (p) {
        xml_unlink_node(p);
        xml_link_node_last(&pold->b, p);
      }
      *p_acc_old = (struct contest_access*) p;
      *p_acc_new = 0;
    }
  }
  for (i = 0; i < CONTEST_LAST_ATTR; i++) {
    if (contest_bool_attr_set[i]) {
      p_b_old = (unsigned char*) contest_desc_get_ptr_nc(pold, attr_to_contest_desc_map[i]);
      p_b_new = (unsigned char*) contest_desc_get_ptr_nc(pnew, attr_to_contest_desc_map[i]);
      *p_b_old = *p_b_new;
    }
  }

  for (i = CONTEST_FIRST_FIELD; i < CONTEST_LAST_FIELD; i++) {
    p = (struct xml_tree*) pnew->fields[i];
    if (p) {
      xml_unlink_node(p);
      xml_link_node_last(&pold->b, p);
    }
    pold->fields[i] = (struct contest_field*) p;
    pnew->fields[i] = 0;
  }
  for (i = 0; i < CONTEST_LAST_MEMBER; i++) {
    p = (struct xml_tree*) pnew->members[i];
    if (p) {
      xml_unlink_node(p);
      xml_link_node_last(&pold->b, p);
    }
    pold->members[i] = (struct contest_member*) p;
    pnew->members[i] = 0;
  }
  p = pnew->caps_node;
  if (p) {
    xml_unlink_node(p);
    xml_link_node_last(&pold->b, p);
  }
  pold->caps_node = p;
  pnew->caps_node = 0;
  pold->capabilities.first = pnew->capabilities.first;
  pnew->capabilities.first = 0;

  if ((p = pnew->slave_rules)) {
    xml_unlink_node(p);
    xml_link_node_last(&pold->b, p);
  }
  pold->slave_rules = p;
  pnew->slave_rules = 0;

  pold->reg_deadline = pnew->reg_deadline;
  pold->start_date = pnew->start_date;
  pold->disable_member_delete = pnew->disable_member_delete;
  pold->last_check_time = pnew->last_check_time;
  pold->last_file_time = pnew->last_file_time;
  pold->user_contest_num = pnew->user_contest_num;

  pold->default_locale_num = l10n_parse_locale(pold->default_locale);
}

int
contests_load(int number, struct contest_desc **p_cnts)
{
  unsigned char c_path[PATH_MAX];
  struct stat sb;
  struct contest_desc *cnts;

  ASSERT(p_cnts);
  *p_cnts = 0;
  contests_make_path(c_path, sizeof(c_path), number);
  if (stat(c_path, &sb) < 0) return -CONTEST_ERR_NO_CONTEST;
  cnts = parse_one_contest_xml(c_path, number, 1);
  if (!cnts) return -CONTEST_ERR_BAD_XML;
  if (cnts->id != number) {
    contests_free(cnts);
    return -CONTEST_ERR_ID_NOT_MATCH;
  }
  *p_cnts = cnts;
  return 0;
}

struct xml_tree *
contests_new_node(int tag)
{
  struct xml_tree *p = xml_elem_alloc(tag, elem_sizes);
  p->tag = tag;
  return p;
}

static int
do_check_ip(struct contest_access *acc, ej_ip_t ip, int ssl)
{
  struct contest_ip *p;

  if (!acc) return 0;
  if (!ip && acc->default_is_allow) return 1;
  if (!ip) return 0;

  for (p = (struct contest_ip*) acc->b.first_down;
       p; p = (struct contest_ip*) p->b.right) {
    if ((ip & p->mask) == p->addr && (p->ssl == -1 || p->ssl == ssl))
      return p->allow;
  }
  return acc->default_is_allow;
}

int
contests_check_ip(int num, int field, ej_ip_t ip, int ssl)
{
  const struct contest_desc *d = 0;
  struct contest_access *acc = 0;
  int e;

  if ((e = contests_get(num, &d)) < 0) {
    err("contests_check_ip: %d: %s", num, contests_strerror(-e));
    return 0;
  }
  switch (field) {
  case CONTEST_REGISTER_ACCESS: acc = d->register_access; break;
  case CONTEST_USERS_ACCESS:    acc = d->users_access; break;
  case CONTEST_MASTER_ACCESS:   acc = d->master_access; break;
  case CONTEST_JUDGE_ACCESS:    acc = d->judge_access; break;
  case CONTEST_TEAM_ACCESS:     acc = d->team_access; break;
  case CONTEST_SERVE_CONTROL_ACCESS: acc = d->serve_control_access; break;
  default:
    err("contests_check_ip: %d: invalid field %d", num, field);
    return 0;
  }
  return do_check_ip(acc, ip, ssl);
}

int
contests_check_register_ip(int num, ej_ip_t ip, int ssl)
{
  return contests_check_ip(num, CONTEST_REGISTER_ACCESS, ip, ssl);
}
int
contests_check_register_ip_2(const struct contest_desc *cnts, ej_ip_t ip, int ssl)
{
  return do_check_ip(cnts->register_access, ip, ssl);
}
int
contests_check_users_ip(int num, ej_ip_t ip, int ssl)
{
  return contests_check_ip(num, CONTEST_USERS_ACCESS, ip, ssl);
}
int
contests_check_users_ip_2(const struct contest_desc *cnts, ej_ip_t ip, int ssl)
{
  return do_check_ip(cnts->users_access, ip, ssl);
}
int
contests_check_master_ip(int num, ej_ip_t ip, int ssl)
{
  return contests_check_ip(num, CONTEST_MASTER_ACCESS, ip, ssl);
}
int
contests_check_master_ip_2(const struct contest_desc *cnts, ej_ip_t ip, int ssl)
{
  return do_check_ip(cnts->master_access, ip, ssl);
}
int
contests_check_judge_ip(int num, ej_ip_t ip, int ssl)
{
  return contests_check_ip(num, CONTEST_JUDGE_ACCESS, ip, ssl);
}
int
contests_check_judge_ip_2(const struct contest_desc *cnts, ej_ip_t ip, int ssl)
{
  return do_check_ip(cnts->judge_access, ip, ssl);
}
int
contests_check_team_ip(int num, ej_ip_t ip, int ssl)
{
  return contests_check_ip(num, CONTEST_TEAM_ACCESS, ip, ssl);
}
int
contests_check_team_ip_2(const struct contest_desc *cnts, ej_ip_t ip, int ssl)
{
  return do_check_ip(cnts->team_access, ip, ssl);
}
int
contests_check_serve_control_ip(int num, ej_ip_t ip, int ssl)
{
  return contests_check_ip(num, CONTEST_SERVE_CONTROL_ACCESS, ip, ssl);
}
int
contests_check_serve_control_ip_2(const struct contest_desc *cnts, ej_ip_t ip, int ssl)
{
  return do_check_ip(cnts->serve_control_access, ip, ssl);
}

struct callback_list_item
{
  struct callback_list_item *next;
  void (*func)(const struct contest_desc *);
};
static struct callback_list_item *load_list;
static struct callback_list_item *unload_list;

static struct callback_list_item *
contests_set_callback(struct callback_list_item *list,
                      void (*f)(const struct contest_desc *))
{
  struct callback_list_item *p = 0;

  if (!f) return list;
  for (p = list; p; p = p->next)
    if (p->func == f)
      return list;

  p = (struct callback_list_item *) xcalloc(1, sizeof(*p));
  p->next = list;
  p->func = f;
  return p;
}

void
contests_set_load_callback(void (*f)(const struct contest_desc *))
{
  load_list = contests_set_callback(load_list, f);
}
void
contests_set_unload_callback(void (*f)(const struct contest_desc *))
{
  unload_list = contests_set_callback(unload_list, f);
}

static unsigned char *contests_dir;
static unsigned int contests_allocd;
static struct contest_desc **contests_desc;

struct contest_desc *
contests_free(struct contest_desc *cnts)
{
  if (!cnts) return 0;
  xml_tree_free((struct xml_tree *) cnts, &contests_parse_spec);
  return 0;
}

void
contests_free_2(struct xml_tree *t)
{
  if (t) xml_tree_free(t, &contests_parse_spec);
}

int
contests_make_path(unsigned char *buf, size_t sz, int num)
{
  return snprintf(buf, sz, "%s/%06d.xml", contests_dir, num);
}

int
contests_set_directory(unsigned char const *dir)
{
  struct stat bbb;

  if (!dir) return -CONTEST_ERR_BAD_DIR;
  if (stat(dir, &bbb) < 0) return -CONTEST_ERR_BAD_DIR;
  if (!S_ISDIR(bbb.st_mode)) return -CONTEST_ERR_BAD_DIR;
  xfree(contests_dir);
  contests_dir = xstrdup(dir);
  return 0;
}

struct get_list_state
{
  time_t last_check_time;
  time_t last_update_time;
  int max_num;
  int u;
  int a;
  int *ids;
  unsigned char *map;
  int map_a;
};
static struct get_list_state gl_state;

static int
int_sort_func(const void *p1, const void *p2)
{
  int i1 = *(const int*) p1;
  int i2 = *(const int*) p2;
  if (i1 < i2) return -1;
  if (i1 > i2) return 1;
  return 0;
}

int
contests_get_list(const int **p_list)
{
  DIR *d = 0;
  struct dirent *dd = 0;
  int i, j;
  struct stat bbb;
  unsigned char c_path[1024];
  time_t cur_time = time(0);

  if (p_list) *p_list = 0;
  if (cur_time <= gl_state.last_check_time) {
    if (p_list) *p_list = gl_state.ids;
    return gl_state.u;
  }
  gl_state.last_check_time = cur_time;
  if (stat(contests_dir, &bbb) < 0) return -CONTEST_ERR_BAD_DIR;
  if (!S_ISDIR(bbb.st_mode)) return -CONTEST_ERR_BAD_DIR;
  if (bbb.st_mtime <= gl_state.last_update_time) {
    if (p_list) *p_list = gl_state.ids;
    return gl_state.u;
  }
  gl_state.last_update_time = cur_time;

  // we don't check specifically for "." or ".."
  if (!(d = opendir(contests_dir))) return -CONTEST_ERR_BAD_DIR;
  gl_state.u = 0;
  gl_state.max_num = 0;
  while ((dd = readdir(d))) {
    if (sscanf(dd->d_name, "%d", &j) != 1 || j <= 0) continue;
    snprintf(c_path, sizeof(c_path), "%06d.xml", j);
    if (strcmp(c_path, dd->d_name)) continue;
    snprintf(c_path, sizeof(c_path), "%s/%06d.xml", contests_dir, j);
    if (access(c_path, R_OK) < 0) continue;
    //if (stat(c_path, &bbb) < 0) continue;
    //if (!S_ISREG(bbb.st_mode)) continue;

    if (gl_state.u == gl_state.a) {
      if (!gl_state.a) gl_state.a = 64;
      gl_state.a *= 2;
      XREALLOC(gl_state.ids, gl_state.a);
    }
    gl_state.ids[gl_state.u++] = j;
    if (j > gl_state.max_num) gl_state.max_num = j;
  }
  closedir(d);
  if (!gl_state.max_num) return 0;

  if (gl_state.max_num < 1000) {
    unsigned char *tmp_map = alloca(gl_state.max_num + 1);
    memset(tmp_map, 0, gl_state.max_num + 1);
    for (i = 0; i < gl_state.u; i++) {
      ASSERT(gl_state.ids[i] > 0 && gl_state.ids[i] <= gl_state.max_num);
      tmp_map[gl_state.ids[i]] = 1;
    }
    j = 0;
    for (i = 0; i <= gl_state.max_num; i++)
      if (tmp_map[i])
        gl_state.ids[j++] = i;
    ASSERT(j == gl_state.u);
  } else {
    qsort(gl_state.ids, gl_state.u, sizeof(gl_state.ids[0]), int_sort_func);
  }

  if (gl_state.max_num >= gl_state.map_a) {
    if (!gl_state.map_a) gl_state.map_a = 32;
    while (gl_state.max_num >= gl_state.map_a) gl_state.map_a *= 2;
    xfree(gl_state.map);
    XCALLOC(gl_state.map, gl_state.map_a);
  } else {
    memset(gl_state.map, 0, gl_state.map_a);
  }

  for (i = 0; i < gl_state.u; i++) {
    ASSERT(gl_state.ids[i] > 0 && gl_state.ids[i] <= gl_state.max_num);
    gl_state.map[gl_state.ids[i]] = 1;
  }
  if (p_list) *p_list = gl_state.ids;
  return gl_state.u;
}

int
contests_get_set(const unsigned char **p_map)
{
  DIR *d = 0;
  struct dirent *dd = 0;
  int i, j;
  struct stat bbb;
  unsigned char c_path[1024];
  time_t cur_time = time(0);

  if (p_map) *p_map = 0;
  if (cur_time <= gl_state.last_check_time) {
    if (p_map) *p_map = gl_state.map;
    return gl_state.max_num + 1;
  }
  gl_state.last_check_time = cur_time;
  if (stat(contests_dir, &bbb) < 0) return -CONTEST_ERR_BAD_DIR;
  if (!S_ISDIR(bbb.st_mode)) return -CONTEST_ERR_BAD_DIR;
  if (bbb.st_mtime <= gl_state.last_update_time) {
    if (p_map) *p_map = gl_state.map;
    return gl_state.max_num + 1;
  }
  gl_state.last_update_time = cur_time;

  // we don't check specifically for "." or ".."
  if (!(d = opendir(contests_dir))) return -CONTEST_ERR_BAD_DIR;
  gl_state.u = 0;
  gl_state.max_num = 0;
  while ((dd = readdir(d))) {
    if (sscanf(dd->d_name, "%d", &j) != 1 || j <= 0) continue;
    snprintf(c_path, sizeof(c_path), "%06d.xml", j);
    if (strcmp(c_path, dd->d_name)) continue;
    snprintf(c_path, sizeof(c_path), "%s/%06d.xml", contests_dir, j);
    if (access(c_path, R_OK) < 0) continue;
    //if (stat(c_path, &bbb) < 0) continue;
    //if (!S_ISREG(bbb.st_mode)) continue;

    if (gl_state.u == gl_state.a) {
      if (!gl_state.a) gl_state.a = 64;
      gl_state.a *= 2;
      XREALLOC(gl_state.ids, gl_state.a);
    }
    gl_state.ids[gl_state.u++] = j;
    if (j > gl_state.max_num) gl_state.max_num = j;
  }
  closedir(d);
  if (!gl_state.max_num) return 0;

  if (gl_state.max_num < 1000) {
    unsigned char *tmp_map = alloca(gl_state.max_num + 1);
    memset(tmp_map, 0, gl_state.max_num + 1);
    for (i = 0; i < gl_state.u; i++) {
      ASSERT(gl_state.ids[i] > 0 && gl_state.ids[i] <= gl_state.max_num);
      tmp_map[gl_state.ids[i]] = 1;
    }
    j = 0;
    for (i = 0; i <= gl_state.max_num; i++)
      if (tmp_map[i])
        gl_state.ids[j++] = i;
    ASSERT(j == gl_state.u);
  } else {
    qsort(gl_state.ids, gl_state.u, sizeof(gl_state.ids[0]), int_sort_func);
  }

  if (gl_state.max_num >= gl_state.map_a) {
    if (!gl_state.map_a) gl_state.map_a = 32;
    while (gl_state.max_num >= gl_state.map_a) gl_state.map_a *= 2;
    xfree(gl_state.map);
    XCALLOC(gl_state.map, gl_state.map_a);
  } else {
    memset(gl_state.map, 0, gl_state.map_a);
  }

  for (i = 0; i < gl_state.u; i++) {
    ASSERT(gl_state.ids[i] > 0 && gl_state.ids[i] <= gl_state.max_num);
    gl_state.map[gl_state.ids[i]] = 1;
  }
  if (p_map) *p_map = gl_state.map;
  return gl_state.max_num + 1;
}

int
contests_get(int number, const struct contest_desc **p_desc)
{
  unsigned char c_path[1024];
  struct stat sb;
  struct contest_desc *cnts;
  time_t cur_time;

  ASSERT(p_desc);
  *p_desc = 0;
  if (number <= 0) return -CONTEST_ERR_BAD_ID;

  if (number >= contests_allocd || !contests_desc[number]) {
    // no previous info about the contest
    contests_make_path(c_path, sizeof(c_path), number);
    if (stat(c_path, &sb) < 0) return -CONTEST_ERR_NO_CONTEST;
    // load the info and adjust time marks
    cnts = parse_one_contest_xml(c_path, number, 0);
    if (!cnts) return -CONTEST_ERR_BAD_XML;
    if (cnts->id != number) {
      contests_free(cnts);
      return -CONTEST_ERR_ID_NOT_MATCH;
    }
    cnts->last_check_time = time(0);
    cnts->last_file_time = sb.st_mtime;
    // extend arrays
    if (number >= contests_allocd) {
      unsigned int new_allocd = contests_allocd;
      struct contest_desc **new_contests = 0;

      if (!new_allocd) new_allocd = 32;
      while (number >= new_allocd) new_allocd *= 2;
      new_contests = xcalloc(new_allocd, sizeof(new_contests[0]));
      if (contests_allocd > 0) {
        memcpy(new_contests, contests_desc,
               contests_allocd * sizeof(new_contests[0]));
      }
      xfree(contests_desc);
      contests_allocd = new_allocd;
      contests_desc = new_contests;
    }
    // put new contest into the array
    contests_desc[number] = cnts;
    *p_desc = cnts;
    return 0;
  }

  cur_time = time(0);
  cnts = contests_desc[number];
  ASSERT(cnts->id == number);
  // check the time since last check
  if (cur_time <= cnts->last_check_time + CONTEST_CHECK_TIME) {
    *p_desc = cnts;
    return 0;
  }

  contests_make_path(c_path, sizeof(c_path), number);
  if (stat(c_path, &sb) < 0) {
    // FIXME: contest removed. what to do?
    contests_free(cnts);
    contests_desc[number] = 0;
    return -CONTEST_ERR_REMOVED;
  }
  // check whether update timestamp is changed
  if (sb.st_mtime == cnts->last_file_time) {
    *p_desc = cnts;
    return 0;
  }

  // load the info and adjust time marks
  cnts = parse_one_contest_xml(c_path, number, 0);
  if (!cnts) return -CONTEST_ERR_BAD_XML;
  if (cnts->id != number) {
    contests_free(cnts);
    return -CONTEST_ERR_ID_NOT_MATCH;
  }
  cnts->last_check_time = time(0);
  cnts->last_file_time = sb.st_mtime;
  /* FIXME: there may be pointers to the current cnts structure
   * outta there, so we should not just free the old contest
   * description
   */
  contests_merge(contests_desc[number], cnts);
  contests_free(cnts);
  *p_desc = contests_desc[number];
  return 0;
}

static unsigned char const * const contests_errors[] =
{
  "no error",
  "invalid contests directory",
  "invalid contest id",
  "contest does not exist",
  "error during XML reading",
  "contest id in the file and file name do not match",
  "contest is removed",
  "cannot create a file in contest directory",
  "i/o error",

  [CONTEST_ERR_LAST] "unknown error"
};

const unsigned char *
contests_strerror(int e)
{
  if (e < 0) e = -e;
  if (e > CONTEST_ERR_LAST) e = CONTEST_ERR_LAST;
  return (const unsigned char *) contests_errors[e];
}

void
contests_write_header(FILE *f, const struct contest_desc *cnts)
{
  const int *flist;
  int i, j;

  fprintf(f,
          "<%s %s=\"%d\"", elem_map[CONTEST_CONTEST],
          attr_map[CONTEST_A_ID], cnts->id);

  flist = (const int[]) {
    CONTEST_A_AUTOREGISTER, CONTEST_A_DISABLE_TEAM_PASSWORD,
    CONTEST_A_SIMPLE_REGISTRATION,
    CONTEST_A_SEND_PASSWD_EMAIL, CONTEST_A_ASSIGN_LOGINS,
    CONTEST_A_FORCE_REGISTRATION, CONTEST_A_DISABLE_NAME,
    CONTEST_A_ENABLE_PASSWORD_RECOVERY, CONTEST_A_EXAM_MODE,
    CONTEST_A_DISABLE_PASSWORD_CHANGE, CONTEST_A_DISABLE_LOCALE_CHANGE,
    CONTEST_A_PERSONAL, CONTEST_A_ALLOW_REG_DATA_EDIT,
    CONTEST_A_DISABLE_MEMBER_DELETE, CONTEST_A_CLOSED, CONTEST_A_INVISIBLE,
    CONTEST_A_MANAGED, CONTEST_A_RUN_MANAGED,
    0
  };
  for (i = 0; flist[i]; ++i) {
    j = attr_to_contest_desc_map[flist[i]];
    if (*(const unsigned char*) contest_desc_get_ptr(cnts, j))
      fprintf(f, "\n         %s=\"%s\"", attr_map[flist[i]], "yes");
  }
  if (!cnts->clean_users) {
    fprintf(f, "\n         %s=\"%s\"",
            attr_map[CONTEST_A_CLEAN_USERS], "no");
  }

  fprintf(f, ">");
}

static void
unparse_access(FILE *f, const struct contest_access *acc, int tag)
{
  struct contest_ip *ip;
  unsigned char ssl_str[64];

  if (!acc) return;
  if (!acc->default_is_allow && !acc->b.first_down) return;
  if (!acc->b.first_down) {
    fprintf(f, "  <%s default=\"%s\"/>\n", elem_map[tag],
            acc->default_is_allow?"allow":"deny");
    return;
  }
  fprintf(f, "  <%s default=\"%s\">\n", elem_map[tag],
          acc->default_is_allow?"allow":"deny");
  for (ip = (typeof(ip)) acc->b.first_down; ip;
       ip = (typeof(ip)) ip->b.right) {
    ssl_str[0] = 0;
    if (ip->ssl >= 0)
      snprintf(ssl_str, sizeof(ssl_str), " %s=\"%s\"",
               attr_map[CONTEST_A_SSL], ip->ssl?"yes":"no");
    fprintf(f, "    <%s %s=\"%s\"%s>%s</%s>\n",
            elem_map[CONTEST_IP], attr_map[CONTEST_A_ALLOW],
            ip->allow?"yes":"no", ssl_str,
            xml_unparse_ip_mask(ip->addr, ip->mask),
            elem_map[CONTEST_IP]);
  }
  fprintf(f, "  </%s>\n", elem_map[tag]);
}

static void
unparse_field(
        FILE *f,
        const struct contest_field *pf,
        int id,
        char const * const field_map[],
        const unsigned char *indent)
{
  unsigned char *txt = 0, *arm_txt = 0;
  size_t arm_sz = 0;

  if (!pf) return;
  txt = pf->legend;
  if (txt && *txt && html_armor_needed(txt, &arm_sz)) {
    arm_txt = (unsigned char*) alloca(arm_sz + 1);
    html_armor_string(txt, arm_txt);
    txt = arm_txt;
  }
  fprintf(f, "%s<%s %s=\"%s\" %s=\"%s\"",
          indent, elem_map[CONTEST_FIELD], attr_map[CONTEST_A_ID],
          field_map[id],
          attr_map[CONTEST_A_MANDATORY],
          pf->mandatory?"yes":"no");
  if (txt && *txt) {
    fprintf(f, ">%s</%s>\n", txt, elem_map[CONTEST_FIELD]);
  } else {
    fprintf(f, "/>\n");
  }
}

static void
unparse_fields(FILE *f, const struct contest_member *memb, int tag)
{
  int i;

  if (!memb) return;
  fprintf(f, "  <%s", elem_map[tag]);
  if (memb->min_count >= 0)
    fprintf(f, " %s=\"%d\"", attr_map[CONTEST_A_MIN], memb->min_count);
  if (memb->max_count >= 0)
    fprintf(f, " %s=\"%d\"", attr_map[CONTEST_A_MAX], memb->max_count);
  if (memb->init_count >= 0)
    fprintf(f, " %s=\"%d\"", attr_map[CONTEST_A_INITIAL], memb->init_count);
  fprintf(f, ">\n");
  for (i = 1; i < CONTEST_LAST_MEMBER_FIELD; i++)
    unparse_field(f, memb->fields[i], i, member_field_map, "    ");
  fprintf(f, "  </%s>\n", elem_map[tag]);
}

static void
unparse_text(FILE *f, int tag, const unsigned char *txt)
{
  size_t arm_sz;
  unsigned char *arm_txt;

  if (!txt) return;
  if (html_armor_needed(txt, &arm_sz)) {
    arm_txt = (unsigned char*) alloca(arm_sz + 1);
    html_armor_string(txt, arm_txt);
    txt = arm_txt;
  }
  fprintf(f, "  <%s>%s</%s>\n", elem_map[tag], txt, elem_map[tag]);
}

static void
unparse_texts(
        FILE *f,
        const struct contest_desc *cnts,
        const int *flist)
{
  for (int i = 0; flist[i]; ++i)
    unparse_text(f, flist[i], *(const unsigned char **) contest_desc_get_ptr(cnts, tag_to_contest_desc_map[flist[i]]));
}

void
contests_unparse(FILE *f,
                 const struct contest_desc *cnts)
{
  const struct opcap_list_item *cap;
  unsigned char *s;
  int i, len, skip_elem;
  struct xml_tree *p;
  path_t tmp1, tmp2;

  contests_write_header(f, cnts);
  fprintf(f, "\n");

  unparse_texts(f, cnts, (const int[]) {
    CONTEST_NAME, CONTEST_NAME_EN, CONTEST_DEFAULT_LOCALE, CONTEST_MAIN_URL,
    CONTEST_KEYWORDS,
    0
  });

  // avoid generating root_dir and conf_dir if their values are default
  skip_elem = 0;
  tmp1[0] = 0;
  if (ejudge_config && ejudge_config->contests_home_dir) {
    snprintf(tmp1, sizeof(tmp1), "%s", ejudge_config->contests_home_dir);
  }
#if defined EJUDGE_CONTESTS_HOME_DIR
  if (!tmp1[0]) {
    snprintf(tmp1, sizeof(tmp1), "%s", EJUDGE_CONTESTS_HOME_DIR);
  }
#endif
  if (tmp1[0] && cnts->root_dir) {
    len = strlen(tmp1);
    snprintf(tmp2, sizeof(tmp2), "%s/%06d", tmp1, cnts->id);
    if (!strcmp(tmp2, cnts->root_dir)) {
      // do nothing, <root_dir> has the default value
      skip_elem = 1;
    } else if (!strncmp(tmp1, cnts->root_dir, len)
               && cnts->root_dir[len] == '/') {
      while (cnts->root_dir[len] == '/') len++;
      unparse_text(f, CONTEST_ROOT_DIR, cnts->root_dir + len);
      skip_elem = 1;
    }
  }
  if (!skip_elem) unparse_text(f, CONTEST_ROOT_DIR, cnts->root_dir);

  skip_elem = 0;
  if (cnts->root_dir && cnts->conf_dir) {
    snprintf(tmp1, sizeof(tmp1), "%s/conf", cnts->root_dir);
    if (!strcmp(tmp1, cnts->conf_dir)) skip_elem = 1;
  }
  if (!skip_elem) unparse_text(f, CONTEST_CONF_DIR, cnts->conf_dir);

  if (cnts->user_contest && cnts->user_contest[0])
    unparse_text(f, CONTEST_USER_CONTEST, cnts->user_contest);
  if (cnts->reg_deadline > 0) {
    fprintf(f, "  <%s>%s</%s>\n", elem_map[CONTEST_REGISTRATION_DEADLINE],
            xml_unparse_date(cnts->reg_deadline),
            elem_map[CONTEST_REGISTRATION_DEADLINE]);
  }
  if (cnts->start_date > 0) {
    fprintf(f, "  <%s>%s</%s>\n", elem_map[CONTEST_START_DATE],
            xml_unparse_date(cnts->start_date),
            elem_map[CONTEST_START_DATE]);
  }

  unparse_texts(f, cnts, (const int[]) {
    CONTEST_REGISTER_EMAIL, CONTEST_REGISTER_URL, CONTEST_TEAM_URL,
    CONTEST_STANDINGS_URL, CONTEST_PROBLEMS_URL, CONTEST_REGISTER_EMAIL_FILE,
    CONTEST_LOGIN_TEMPLATE, CONTEST_LOGIN_TEMPLATE_OPTIONS,
    0,
  });

  for (i = CONTEST_REGISTER_ACCESS; i <= CONTEST_SERVE_CONTROL_ACCESS; ++i) {
    unparse_access(f, *(const struct contest_access**) contest_desc_get_ptr(cnts, tag_to_contest_desc_map[i]), i);
  }

  if (cnts->caps_node) {
    fprintf(f, "  <%s>\n", elem_map[CONTEST_CAPS]);
    for (cap = CNTS_FIRST_PERM(cnts); cap; cap = CNTS_NEXT_PERM(cap)) {
      fprintf(f, "    <%s %s = \"%s\">\n",
              elem_map[CONTEST_CAP], attr_map[CONTEST_A_LOGIN], cap->login);
      s = opcaps_unparse(6, 60, cap->caps);
      fprintf(f, "%s", s);
      xfree(s);
      fprintf(f, "    </%s>\n", elem_map[CONTEST_CAP]);
    }
    fprintf(f, "  </%s>\n", elem_map[CONTEST_CAPS]);
  }

  for (i = 1; i < CONTEST_LAST_FIELD; i++) {
    unparse_field(f, cnts->fields[i], i, field_map, "  ");
  }

  for (i = CONTEST_M_CONTESTANT; i <= CONTEST_M_GUEST; ++i) {
    unparse_fields(f, cnts->members[i], CONTEST_CONTESTANTS + i);
  }

  unparse_texts(f, cnts, (const int[]) {
    CONTEST_USERS_HEADER_FILE, CONTEST_USERS_FOOTER_FILE,
    CONTEST_REGISTER_HEADER_FILE, CONTEST_REGISTER_FOOTER_FILE,
    CONTEST_TEAM_HEADER_FILE, CONTEST_TEAM_MENU_1_FILE,
    CONTEST_TEAM_MENU_2_FILE, CONTEST_TEAM_MENU_3_FILE,
    CONTEST_TEAM_SEPARATOR_FILE, CONTEST_TEAM_FOOTER_FILE,
    CONTEST_COPYRIGHT_FILE, CONTEST_PRIV_HEADER_FILE, CONTEST_PRIV_FOOTER_FILE,
    CONTEST_WELCOME_FILE, CONTEST_REG_WELCOME_FILE,

    CONTEST_USERS_HEAD_STYLE, CONTEST_USERS_PAR_STYLE,
    CONTEST_USERS_TABLE_STYLE, CONTEST_USERS_VERB_STYLE,
    CONTEST_USERS_TABLE_FORMAT, CONTEST_USERS_TABLE_FORMAT_EN,
    CONTEST_USERS_TABLE_LEGEND, CONTEST_USERS_TABLE_LEGEND_EN,
    CONTEST_REGISTER_HEAD_STYLE, CONTEST_REGISTER_PAR_STYLE,
    CONTEST_REGISTER_TABLE_STYLE, CONTEST_TEAM_HEAD_STYLE,
    CONTEST_TEAM_PAR_STYLE,

    CONTEST_SERVE_USER, CONTEST_SERVE_GROUP,
    CONTEST_RUN_USER, CONTEST_RUN_GROUP,

    CONTEST_DIR_MODE, CONTEST_DIR_GROUP,
    CONTEST_FILE_MODE, CONTEST_FILE_GROUP,

    CONTEST_USER_NAME_COMMENT, CONTEST_ALLOWED_LANGUAGES,
    CONTEST_ALLOWED_REGIONS, CONTEST_CF_NOTIFY_EMAIL,
    CONTEST_CLAR_NOTIFY_EMAIL, CONTEST_DAILY_STAT_EMAIL,

    0,
  });

  if (cnts->slave_rules) {
    fprintf(f, "  <%s>\n", elem_map[CONTEST_SLAVE_RULES]);
    for (p = cnts->slave_rules->first_down; p; p = p->right) {
      fprintf(f, "    <%s>%s</%s>\n",
              elem_map[p->tag], p->text, elem_map[p->tag]);
    }
    fprintf(f, "  </%s>\n", elem_map[CONTEST_SLAVE_RULES]);
  }
  fprintf(f, "</%s>", elem_map[CONTEST_CONTEST]);
}

int
contests_save_xml(struct contest_desc *cnts,
                  const unsigned char *txt1,
                  const unsigned char *txt2,
                  const unsigned char *txt3)
{
  int serial = 1;
  unsigned char tmp_path[1024];
  unsigned char xml_path[1024];
  int fd;
  FILE *f;
  struct stat xml_stat;

  while (1) {
    snprintf(tmp_path, sizeof(tmp_path), "%s/_contests_tmp_%d.xml",
             contests_dir, serial++);
    if ((fd = open(tmp_path, O_WRONLY| O_CREAT| O_TRUNC|O_EXCL, 0600)) >= 0)
      break;
    if (errno != EEXIST) return -CONTEST_ERR_FILE_CREATION_ERROR;
  }
  if (!(f = fdopen(fd, "w"))) {
    close(fd);
    unlink(tmp_path);
    return -CONTEST_ERR_FILE_CREATION_ERROR;
  }

  fputs(txt1, f);
  contests_write_header(f, cnts);
  fputs(txt2, f);
  fputs(txt3, f);
  if (ferror(f)) {
    fclose(f);
    unlink(tmp_path);
    return -CONTEST_ERR_IO_ERROR;
  }
  if (fclose(f) < 0) {
    unlink(tmp_path);
    return -CONTEST_ERR_IO_ERROR;
  }

  contests_make_path(xml_path, sizeof(xml_path), cnts->id);
  if (stat(xml_path, &xml_stat) < 0) {
    unlink(tmp_path);
    return -CONTEST_ERR_NO_CONTEST;
  }
  if (!S_ISREG(xml_stat.st_mode)) {
    unlink(tmp_path);
    return -CONTEST_ERR_NO_CONTEST;
  }

  // try to change the owner, but ignore the error
  chown(tmp_path, xml_stat.st_uid, -1);
  // try to change the group and log errors
  if (chown(tmp_path, -1, xml_stat.st_gid) < 0) {
    err("contests_save_xml: chgrp failed: %s", os_ErrorMsg());
  }
  // try to change permissions and log errors
  if (chmod(tmp_path, xml_stat.st_mode & 07777) < 0) {
    err("contests_save_xml: chmod failed: %s", os_ErrorMsg());
  }

  if (rename(tmp_path, xml_path) < 0) {
    err("contests_save_xml: rename failed: %s", os_ErrorMsg());
    unlink(tmp_path);
    return -CONTEST_ERR_FILE_CREATION_ERROR;
  }
  return 0;
}

int
contests_unparse_and_save(
        struct contest_desc *cnts,
        const unsigned char *charset,
        const unsigned char *header,
        const unsigned char *footer,
        const unsigned char *add_footer,
        unsigned char *(*diff_func)(const unsigned char *,
                                    const unsigned char *),
        unsigned char **p_diff_txt)
{
  int serial = 1;
  unsigned char tmp_path[1024];
  unsigned char xml_path[1024];
  int fd;
  FILE *f;
  struct stat xml_stat;
  char *old_text = 0;
  size_t old_size = 0;
  char *new_text = 0;
  size_t new_size = 0;
  unsigned char *diff_txt = 0;

  if (!charset || !*charset) charset = INTERNAL_CHARSET;

  f = open_memstream(&new_text, &new_size);
  fprintf(f, "<?xml version=\"1.0\" encoding=\"%s\" ?>\n", charset);
  fputs(header, f);
  contests_unparse(f, cnts);
  fputs(footer, f);
  fclose(f); f = 0;

  contests_make_path(xml_path, sizeof(xml_path), cnts->id);

  // read the previuos file and compare it with the new
  if (generic_read_file(&old_text, 0, &old_size, 0, 0, xml_path, 0) >= 0
      && new_size == old_size && memcmp(new_text, old_text, new_size) == 0) {
    info("contest_save_xml: %d is not changed", cnts->id);
    xfree(old_text);
    xfree(new_text);
    return 0;
  }
  xfree(old_text); old_text = 0;
  old_size = 0;

  while (1) {
    snprintf(tmp_path, sizeof(tmp_path), "%s/_contests_tmp_%d.xml",
             contests_dir, serial++);
    if ((fd = open(tmp_path, O_WRONLY| O_CREAT| O_TRUNC|O_EXCL, 0600)) >= 0)
      break;
    if (errno != EEXIST) {
      xfree(new_text);
      return -CONTEST_ERR_FILE_CREATION_ERROR;
    }
  }
  if (!(f = fdopen(fd, "w"))) {
    close(fd);
    xfree(new_text);
    unlink(tmp_path);
    return -CONTEST_ERR_FILE_CREATION_ERROR;
  }

  fwrite(new_text, 1, new_size, f);
  xfree(new_text); new_text = 0;
  new_size = 0;
  fputs(add_footer, f);
  if (ferror(f)) {
    fclose(f);
    unlink(tmp_path);
    return -CONTEST_ERR_IO_ERROR;
  }
  if (fclose(f) < 0) {
    unlink(tmp_path);
    return -CONTEST_ERR_IO_ERROR;
  }

  if (diff_func && p_diff_txt) {
    diff_txt = (*diff_func)(xml_path, tmp_path);
  }

  if (stat(xml_path, &xml_stat) >= 0) {
    if (!S_ISREG(xml_stat.st_mode)) {
      unlink(tmp_path);
      xfree(diff_txt);
      return -CONTEST_ERR_NO_CONTEST;
    }

    // try to change the owner, but ignore the error
    chown(tmp_path, xml_stat.st_uid, -1);
    // try to change the group and log errors
    if (chown(tmp_path, -1, xml_stat.st_gid) < 0) {
      err("contests_save_xml: chgrp failed: %s", os_ErrorMsg());
    }
    // try to change permissions and log errors
    if (chmod(tmp_path, xml_stat.st_mode & 07777) < 0) {
      err("contests_save_xml: chmod failed: %s", os_ErrorMsg());
    }
  } else {
    if (chmod(tmp_path, 0664) < 0) {
      err("contests_save_xml: chmod failed: %s", os_ErrorMsg());
    }
  }

  if (rename(tmp_path, xml_path) < 0) {
    err("contests_save_xml: rename failed: %s", os_ErrorMsg());
    unlink(tmp_path);
    xfree(diff_txt);
    return -CONTEST_ERR_FILE_CREATION_ERROR;
  }
  if (p_diff_txt) *p_diff_txt = diff_txt;
  return 0;
}

void
contests_get_path_in_conf_dir(
        unsigned char *buf,
        size_t size,
        const struct contest_desc *cnts,
        const unsigned char *file)
{
  path_t home_dir;
  path_t root_dir;
  path_t conf_dir;

  if (os_IsAbsolutePath(file)) {
    snprintf(buf, size, "%s", file);
    return;
  }

  if (cnts && cnts->conf_dir && os_IsAbsolutePath(cnts->conf_dir)) {
    snprintf(buf, size, "%s/%s", cnts->conf_dir, file);
    return;
  }

  if (cnts && cnts->root_dir && os_IsAbsolutePath(cnts->root_dir)) {
    snprintf(root_dir, sizeof(root_dir), "%s", cnts->root_dir);
  } else {
    home_dir[0] = 0;
#if defined CONTESTS_HOME_DIR
    snprintf(home_dir, sizeof(home_dir), "%s", CONTESTS_HOME_DIR);
#endif
    if (!home_dir[0]) {
      snprintf(home_dir, sizeof(home_dir), "%s", "/home/judges");
    }
    if (cnts->root_dir) {
      snprintf(root_dir, sizeof(root_dir), "%s/%s", home_dir, cnts->root_dir);
    } else {
      snprintf(root_dir, sizeof(root_dir), "%s/%06d", home_dir, cnts->id);
    }
  }

  if (cnts && cnts->conf_dir) {
    snprintf(conf_dir, sizeof(conf_dir), "%s/%s", root_dir, cnts->conf_dir);
  } else {
    snprintf(conf_dir, sizeof(conf_dir), "%s/conf", root_dir);
  }
  snprintf(buf, size, "%s/%s", conf_dir, file);
}

static const unsigned char *const form_field_names[] =
{
  [CONTEST_F_HOMEPAGE] = "Home page",
  [CONTEST_F_PHONE] = "Phone",
  [CONTEST_F_INST] = "Institution",
  [CONTEST_F_INST_EN] = "Institution (English)",
  [CONTEST_F_INSTSHORT] = "Institution, short",
  [CONTEST_F_INSTSHORT_EN] = "Institution, short (English)",
  [CONTEST_F_INSTNUM] = "Institution number",
  [CONTEST_F_FAC] = "Faculty",
  [CONTEST_F_FAC_EN] = "Faculty (English)",
  [CONTEST_F_FACSHORT] = "Faculty, short",
  [CONTEST_F_FACSHORT_EN] = "Faculty, short (English)",
  [CONTEST_F_CITY] = "City",
  [CONTEST_F_CITY_EN] = "City (English)",
  [CONTEST_F_COUNTRY] = "Country",
  [CONTEST_F_COUNTRY_EN] = "Country (English)",
  [CONTEST_F_REGION] = "Region",
  [CONTEST_F_AREA] = "Area",
  [CONTEST_F_ZIP] = "Zip code",
  [CONTEST_F_STREET] = "Street address",
  [CONTEST_F_LANGUAGES] = "Programming Languages",
  [CONTEST_F_FIELD0] = "Field 0",
  [CONTEST_F_FIELD1] = "Field 1",
  [CONTEST_F_FIELD2] = "Field 2",
  [CONTEST_F_FIELD3] = "Field 3",
  [CONTEST_F_FIELD4] = "Field 4",
  [CONTEST_F_FIELD5] = "Field 5",
  [CONTEST_F_FIELD6] = "Field 6",
  [CONTEST_F_FIELD7] = "Field 7",
  [CONTEST_F_FIELD8] = "Field 8",
  [CONTEST_F_FIELD9] = "Field 9",
};
const unsigned char *
contests_get_form_field_name(int ff)
{
  ASSERT(ff > 0 && ff < CONTEST_LAST_FIELD);
  return form_field_names[ff];
}

static const unsigned char *const member_field_names[] =
{
  [CONTEST_MF_FIRSTNAME] = "First Name",
  [CONTEST_MF_FIRSTNAME_EN] = "First Name (English)",
  [CONTEST_MF_MIDDLENAME] = "Middle Name",
  [CONTEST_MF_MIDDLENAME_EN] = "Middle Name (English)",
  [CONTEST_MF_SURNAME] = "Surname",
  [CONTEST_MF_SURNAME_EN] = "Surname (English)",
  [CONTEST_MF_STATUS] = "Status",
  [CONTEST_MF_GENDER] = "Gender",
  [CONTEST_MF_GRADE] = "Grade",
  [CONTEST_MF_GROUP] = "Group",
  [CONTEST_MF_GROUP_EN] = "Group (English)",
  [CONTEST_MF_EMAIL] = "E-mail",
  [CONTEST_MF_HOMEPAGE] = "Homepage",
  [CONTEST_MF_PHONE] = "Phone",
  [CONTEST_MF_INST] = "Institution",
  [CONTEST_MF_INST_EN] = "Institution (English)",
  [CONTEST_MF_INSTSHORT] = "Institution, short",
  [CONTEST_MF_INSTSHORT_EN] = "Institution, short (English)",
  [CONTEST_MF_FAC] = "Faculty",
  [CONTEST_MF_FAC_EN] = "Faculty (English)",
  [CONTEST_MF_FACSHORT] = "Faculty, short",
  [CONTEST_MF_FACSHORT_EN] = "Faculty, short (English)",
  [CONTEST_MF_OCCUPATION] = "Occupation",
  [CONTEST_MF_OCCUPATION_EN] = "Occupation (English)",
  [CONTEST_MF_DISCIPLINE] = "Discipline",
  [CONTEST_MF_BIRTH_DATE] = "Birth date",
  [CONTEST_MF_ENTRY_DATE] = "Entry date",
  [CONTEST_MF_GRADUATION_DATE] = "Graduation date",
};
const unsigned char *
contests_get_member_field_name(int ff)
{
  ASSERT(ff > 0 && ff < CONTEST_LAST_MEMBER_FIELD);
  return member_field_names[ff];
}

static const unsigned char *const member_names[] =
{
  [CONTEST_M_CONTESTANT] = "Contestant",
  [CONTEST_M_RESERVE] = "Reserve",
  [CONTEST_M_COACH] = "Coach",
  [CONTEST_M_ADVISOR] = "Advisor",
  [CONTEST_M_GUEST] = "Guest",
};
const unsigned char *
contests_get_member_name(int ff)
{
  ASSERT(ff >= 0 && ff < CONTEST_LAST_MEMBER);
  return member_names[ff];
}

int
contests_remove_nth_permission(struct contest_desc *cnts, int n)
{
  struct opcap_list_item *perms;
  int j;

  for (j = 0, perms = CNTS_FIRST_PERM(cnts);
       perms && n != j;
       perms = CNTS_NEXT_PERM_NC(perms), ++j)
  if (!perms || n != j) return -1;

  xml_unlink_node(&perms->b);
  contests_free_2(&perms->b);
  cnts->capabilities.first=(struct opcap_list_item*)cnts->caps_node->first_down;
  if (!cnts->capabilities.first) {
    xml_unlink_node(cnts->caps_node);
    contests_free_2(cnts->caps_node);
    cnts->caps_node = 0;
  }
  return 0;
}

int
contests_add_permission(
        struct contest_desc *cnts,
        const unsigned char *login,
        opcap_t caps)
{
  struct opcap_list_item *cap_node;

  for (cap_node = CNTS_FIRST_PERM(cnts); cap_node;
       cap_node = CNTS_NEXT_PERM_NC(cap_node))
    if (!strcmp(cap_node->login, login)) {
      cap_node->caps |= caps;
      return 0;
    }

  if (!cnts->caps_node) {
    cnts->caps_node = contests_new_node(CONTEST_CAPS);
    xml_link_node_last(&cnts->b, cnts->caps_node);
  }
  cap_node = (typeof(cap_node)) contests_new_node(CONTEST_CAP);
  if (!cnts->capabilities.first) cnts->capabilities.first = cap_node;
  cap_node->login = xstrdup(login);
  cap_node->caps = caps;
  xml_link_node_last(cnts->caps_node, &cap_node->b);
  return 1;
}

void
contests_copy_permissions(
        struct contest_desc *cdst,
        const struct contest_desc *csrc)
{
  struct opcap_list_item *dperms1, *dperms2;
  const struct opcap_list_item *sperms;

  if (!cdst || !csrc) return;

  // remove all permissions from cdst
  for (dperms1 = CNTS_FIRST_PERM(cdst); dperms1; dperms1 = dperms2) {
    dperms2 = CNTS_NEXT_PERM_NC(dperms1);
    xml_unlink_node(&dperms1->b);
    contests_free_2(&dperms1->b);
  }
  if (cdst->caps_node) {
    xml_unlink_node(cdst->caps_node);
    contests_free_2(cdst->caps_node);
  }
  cdst->caps_node = 0;
  cdst->capabilities.first = 0;

  if (!csrc->capabilities.first) return;

  // copy all permissions from csrc to cdst
  cdst->caps_node = contests_new_node(CONTEST_CAPS);
  xml_link_node_last(&cdst->b, cdst->caps_node);

  for (sperms = CNTS_FIRST_PERM(csrc); sperms;
       sperms = CNTS_NEXT_PERM(sperms)) {
    dperms1 = (struct opcap_list_item *) contests_new_node(CONTEST_CAP);
    if (!cdst->capabilities.first) cdst->capabilities.first = dperms1;
    dperms1->login = xstrdup(sperms->login);
    dperms1->caps = sperms->caps;
    xml_link_node_last(cdst->caps_node, &dperms1->b);
  }
}

int
contests_set_permission(
        struct contest_desc *cnts,
        int num,
        opcap_t caps)
{
  struct opcap_list_item *p;
  int i;

  for (i = 0, p = CNTS_FIRST_PERM(cnts); i != num && p;
       ++i, p = CNTS_NEXT_PERM_NC(p));
  if (i != num || !p) return -1;
  p->caps = caps;
  return 0;
}

void
contests_set_default(
        struct contest_desc *cnts,
        struct contest_access **p_acc,
        int tag,
        int default_is_allow)
{
  struct contest_access *new_acc = 0;
  
  if (*p_acc && (*p_acc)->b.first_down) {
    (*p_acc)->default_is_allow = !!default_is_allow;
    return;
  }
  if (!default_is_allow) {
    if (!*p_acc) return;
    (*p_acc)->default_is_allow = 0;
    if (!(*p_acc)->b.first_down) {
      xml_unlink_node(&(*p_acc)->b);
      contests_free_2(&(*p_acc)->b);
      *p_acc = 0;
    }
    return;
  }

  if (!*p_acc) {
    new_acc = (struct contest_access*) contests_new_node(tag);
    xml_link_node_last(&cnts->b, &new_acc->b);
    *p_acc = new_acc;
  }
  (*p_acc)->default_is_allow = default_is_allow;
}
void
contests_add_ip(
        struct contest_desc *cnts,
        struct contest_access **p_acc,
        int tag,
        ej_ip_t addr,
        ej_ip_t mask,
        int ssl_flag,
        int default_allow)
{
  struct contest_access *new_acc = 0;
  struct contest_ip *new_ip;

  if (!*p_acc) {
    new_acc = (struct contest_access*) contests_new_node(tag);
    xml_link_node_last(&cnts->b, &new_acc->b);
    *p_acc = new_acc;
  }
  new_ip = (struct contest_ip*) contests_new_node(CONTEST_IP);
  new_ip->addr = addr;
  new_ip->mask = mask;
  new_ip->allow = default_allow;
  new_ip->ssl = ssl_flag;
  xml_link_node_last(&(*p_acc)->b, &new_ip->b);
}

struct contest_ip *
contests_get_ip_rule_nc(
        struct contest_access *acc,
        int n)
{
  struct contest_ip *p;
  int i;

  if (!acc) return 0;

  for (i = 0, p = CNTS_FIRST_IP_NC(acc); p && i != n;
       ++i, p = CNTS_NEXT_IP_NC(p));
  if (p && i == n) return p;
  return 0;
}

int
contests_delete_ip_rule(
        struct contest_access **p_acc,
        int n)
{
  int i;
  struct contest_ip *p;

  if (!*p_acc) return -1;

  for (i = 0, p = CNTS_FIRST_IP_NC(*p_acc); i != n && p;
       ++i, p = CNTS_NEXT_IP_NC(p));
  if (!p || i != n) return -1;

  xml_unlink_node(&p->b);
  contests_free_2(&p->b);
  if (!CNTS_FIRST_IP(*p_acc) && !(*p_acc)->default_is_allow) {
    xml_unlink_node(&(*p_acc)->b);
    contests_free_2(&(*p_acc)->b);
    *p_acc = 0;
  }
  return 0;
}

static void
swap_tree_nodes(struct xml_tree *first)
{
  struct xml_tree *second;
  struct xml_tree *top;
  struct xml_tree *before_first;
  struct xml_tree *after_second;

  ASSERT(first);
  second = first->right;
  ASSERT(second);
  ASSERT(second->left == first);
  top = first->up;
  ASSERT(top == second->up);
  before_first = first->left;
  after_second = second->right;
  first->left = second;
  first->right = after_second;
  second->left = before_first;
  second->right = first;
  if (!before_first) {
    ASSERT(top->first_down == first);
    top->first_down = second;
  } else {
    before_first->right = second;
  }
  if (!after_second) {
    ASSERT(top->last_down == second);
    top->last_down = first;
  } else {
    after_second->left = first;
  }
}

int
contests_forward_ip_rule(
        struct contest_access **p_acc,
        int n)
{
  int i;
  struct contest_ip *p;
  struct contest_access *acc = *p_acc;

  if (!acc) return -1;

  for (i = 0, p = CNTS_FIRST_IP_NC(acc); i != n && p;
       ++i, p = CNTS_NEXT_IP_NC(p));
  if (!p || i != n) return -1;
  if (!p->b.left) return -1;
  swap_tree_nodes(p->b.left);
  return 0;
}

int
contests_backward_ip_rule(
        struct contest_access **p_acc,
        int n)
{
  int i;
  struct contest_ip *p;
  struct contest_access *acc = *p_acc;

  if (!acc) return -1;

  for (i = 0, p = CNTS_FIRST_IP_NC(acc); i != n && p;
       ++i, p = CNTS_NEXT_IP_NC(p));
  if (!p || i != n) return -1;
  if (!p->b.right) return -1;
  swap_tree_nodes(&p->b);
  return 0;
}

int
contests_set_general_field(
        struct contest_desc *cnts,
        int field_id,
        int opt_val,
        const unsigned char *legend)
{
  struct contest_field *p;

  ASSERT(cnts);
  ASSERT(field_id > 0 && field_id < CONTEST_LAST_FIELD);
  ASSERT(opt_val >= 0 && opt_val <= 2);

  if (!opt_val) {
    if (cnts->fields[field_id]) {
      xml_unlink_node(&cnts->fields[field_id]->b);
      contests_free_2(&cnts->fields[field_id]->b);
      cnts->fields[field_id] = 0;
    }
  } else {
    if (!(p = cnts->fields[field_id])) {
      p = (struct contest_field *) contests_new_node(CONTEST_FIELD);
      p->id = field_id;
      cnts->fields[field_id] = p;
      xml_link_node_last(&cnts->b, &p->b);
    }
    p->mandatory = 0;
    if (opt_val == 2) p->mandatory = 1;
    if (!p->legend || p->legend != legend) {
      xfree(p->legend); p->legend = 0;
      if (legend) p->legend = xstrdup(legend);
    }
  }
  return 0;
}

void
contests_delete_member_fields(
        struct contest_desc *cnts,
        int m_id)
{
  struct contest_member *memb;
  int ff;

  ASSERT(cnts);
  ASSERT(m_id >= 0 && m_id < CONTEST_LAST_MEMBER);

  if (!(memb = cnts->members[m_id])) return;
  for (ff = 0; ff < CONTEST_LAST_MEMBER_FIELD; ++ff)
    if (memb->fields[ff]) {
      xml_unlink_node(&memb->fields[ff]->b);
      contests_free_2(&memb->fields[ff]->b);
      memb->fields[ff] = 0;
    }
  xml_unlink_node(&memb->b);
  contests_free_2(&memb->b);
  cnts->members[m_id] = 0;
}

void
contests_set_member_counts(
        struct contest_desc *cnts,
        int m_id,
        int min_count,
        int max_count,
        int init_count)
{
  struct contest_member *memb;

  ASSERT(cnts);
  ASSERT(m_id >= 0 || m_id < CONTEST_LAST_MEMBER);
  ASSERT(min_count >= 0 && min_count <= 5);
  ASSERT(max_count >= 0 && max_count <= 5);
  ASSERT(init_count >= 0 && init_count <= 5);
  ASSERT(min_count <= max_count);

  if (!(memb = cnts->members[m_id]) && !min_count && !max_count && !init_count)
    return;
  if (!memb) {
    memb = (struct contest_member*) contests_new_node(CONTEST_CONTESTANTS+m_id);
    xml_link_node_last(&cnts->b, &memb->b);
    cnts->members[m_id] = memb;
  }
  memb->min_count = min_count;
  memb->max_count = max_count;
  memb->init_count = init_count;
}

void
contests_set_member_field(
        struct contest_desc *cnts,
        int m_id,
        int field_id,
        int opt_val,
        const unsigned char *legend)
{
  struct contest_member *memb;
  struct contest_field *p;

  ASSERT(cnts);
  ASSERT(m_id >= 0 && m_id < CONTEST_LAST_MEMBER);
  ASSERT(field_id > 0 && field_id < CONTEST_LAST_MEMBER_FIELD);

  if (!(memb = cnts->members[m_id])) {
    memb = (struct contest_member*) contests_new_node(CONTEST_CONTESTANTS+m_id);
    xml_link_node_last(&cnts->b, &memb->b);
    cnts->members[m_id] = memb;
  }

  if (!opt_val) {
    if (!memb->fields[field_id]) return;
    xml_unlink_node(&memb->fields[field_id]->b);
    contests_free_2(&memb->fields[field_id]->b);
    memb->fields[field_id] = 0;
    return;
  }
  if (!(p = memb->fields[field_id])) {
    p = (struct contest_field*) contests_new_node(CONTEST_FIELD);
    p->id = field_id;
    memb->fields[field_id] = p;
    xml_link_node_last(&memb->b, &p->b);
  }
  p->mandatory = 0;
  if (opt_val == 2) p->mandatory = 1;
  if (!p->legend || p->legend != legend) {
    xfree(p->legend); p->legend = 0;
    if (legend) p->legend = xstrdup(legend);
  }
}

/*
 * Local variables:
 *  compile-command: "make"
 *  c-font-lock-extra-types: ("\\sw+_t" "FILE" "DIR")
 * End:
 */
