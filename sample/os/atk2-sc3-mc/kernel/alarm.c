/*
 *  TOPPERS ATK2
 *      Toyohashi Open Platform for Embedded Real-Time Systems
 *      Automotive Kernel Version 2
 *
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2004-2015 by Center for Embedded Computing Systems
 *              Graduate School of Information Science, Nagoya Univ., JAPAN
 *  Copyright (C) 2011-2015 by FUJI SOFT INCORPORATED, JAPAN
 *  Copyright (C) 2011-2013 by Spansion LLC, USA
 *  Copyright (C) 2011-2015 by NEC Communication Systems, Ltd., JAPAN
 *  Copyright (C) 2011-2015 by Panasonic Advanced Technology Development Co., Ltd., JAPAN
 *  Copyright (C) 2011-2014 by Renesas Electronics Corporation, JAPAN
 *  Copyright (C) 2011-2015 by Sunny Giken Inc., JAPAN
 *  Copyright (C) 2011-2015 by TOSHIBA CORPORATION, JAPAN
 *  Copyright (C) 2004-2015 by Witz Corporation
 *  Copyright (C) 2014-2015 by AISIN COMCRUISE Co., Ltd., JAPAN
 *  Copyright (C) 2014-2015 by eSOL Co.,Ltd., JAPAN
 *  Copyright (C) 2014-2015 by SCSK Corporation, JAPAN
 *  Copyright (C) 2015 by SUZUKI MOTOR CORPORATION
 *
 *  上記著作権者は，以下の(1)〜(4)の条件を満たす場合に限り，本ソフトウェ
 *  ア（本ソフトウェアを改変したものを含む．以下同じ）を使用・複製・改
 *  変・再配布（以下，利用と呼ぶ）することを無償で許諾する．
 *  (1) 本ソフトウェアをソースコードの形で利用する場合には，上記の著作
 *      権表示，この利用条件および下記の無保証規定が，そのままの形でソー
 *      スコード中に含まれていること．
 *  (2) 本ソフトウェアを，ライブラリ形式など，他のソフトウェア開発に使
 *      用できる形で再配布する場合には，再配布に伴うドキュメント（利用
 *      者マニュアルなど）に，上記の著作権表示，この利用条件および下記
 *      の無保証規定を掲載すること．
 *  (3) 本ソフトウェアを，機器に組み込むなど，他のソフトウェア開発に使
 *      用できない形で再配布する場合には，次のいずれかの条件を満たすこ
 *      と．
 *    (a) 再配布に伴うドキュメント（利用者マニュアルなど）に，上記の著
 *        作権表示，この利用条件および下記の無保証規定を掲載すること．
 *    (b) 再配布の形態を，別に定める方法によって，TOPPERSプロジェクトに
 *        報告すること．
 *  (4) 本ソフトウェアの利用により直接的または間接的に生じるいかなる損
 *      害からも，上記著作権者およびTOPPERSプロジェクトを免責すること．
 *      また，本ソフトウェアのユーザまたはエンドユーザからのいかなる理
 *      由に基づく請求からも，上記著作権者およびTOPPERSプロジェクトを
 *      免責すること．
 *
 *  本ソフトウェアは，AUTOSAR（AUTomotive Open System ARchitecture）仕
 *  様に基づいている．上記の許諾は，AUTOSARの知的財産権を許諾するもので
 *  はない．AUTOSARは，AUTOSAR仕様に基づいたソフトウェアを商用目的で利
 *  用する者に対して，AUTOSARパートナーになることを求めている．
 *
 *  本ソフトウェアは，無保証で提供されているものである．上記著作権者お
 *  よびTOPPERSプロジェクトは，本ソフトウェアに関して，特定の使用目的
 *  に対する適合性も含めて，いかなる保証も行わない．また，本ソフトウェ
 *  アの利用により直接的または間接的に生じたいかなる損害に関しても，そ
 *  の責任を負わない．
 *
 *  $Id: alarm.c 425 2015-12-07 08:06:19Z witz-itoyo $
 */

/*
 *		アラーム管理モジュール
 */

#include "kernel_impl.h"
#include "check.h"
#include "alarm.h"
#include "mc.h"

/*
 *  トレースログマクロのデフォルト定義
 */
#ifndef LOG_GETALB_ENTER
#define LOG_GETALB_ENTER(almid)
#endif /* LOG_GETALB_ENTER */

#ifndef LOG_GETALB_LEAVE
#define LOG_GETALB_LEAVE(ercd, info)
#endif /* LOG_GETALB_LEAVE */

#ifndef LOG_GETALM_ENTER
#define LOG_GETALM_ENTER(almid)
#endif /* LOG_GETALM_ENTER */

#ifndef LOG_GETALM_LEAVE
#define LOG_GETALM_LEAVE(ercd, p_tick)
#endif /* LOG_GETALM_LEAVE */

#ifndef LOG_SETREL_ENTER
#define LOG_SETREL_ENTER(almid, incr, cycle)
#endif /* LOG_SETREL_ENTER */

#ifndef LOG_SETREL_LEAVE
#define LOG_SETREL_LEAVE(ercd)
#endif /* LOG_SETREL_LEAVE */

#ifndef LOG_SETABS_ENTER
#define LOG_SETABS_ENTER(almid, start, cycle)
#endif /* LOG_SETABS_ENTER */

#ifndef LOG_SETABS_LEAVE
#define LOG_SETABS_LEAVE(ercd)
#endif /* LOG_SETABS_LEAVE */

#ifndef LOG_CANALM_ENTER
#define LOG_CANALM_ENTER(almid)
#endif /* LOG_CANALM_ENTER */

#ifndef LOG_CANALM_LEAVE
#define LOG_CANALM_LEAVE(ercd)
#endif /* LOG_CANALM_LEAVE */

#ifndef LOG_ALM_ENTER
#define LOG_ALM_ENTER(p_cntexpinfo)
#endif /* LOG_ALM_ENTER */

#ifndef LOG_ALM_LEAVE
#define LOG_ALM_LEAVE(p_cntexpinfo)
#endif /* LOG_ALM_LEAVE */

/*
 *  アラーム機能の初期化
 */
#ifdef TOPPERS_alarm_initialize

void
alarm_initialize(void)
{
	AlarmType	i;
	ALMCB		*p_almcb;
	CoreIdType	coreid = x_core_id();

	for (i = 0U; i < tnum_alarm; i++) {
		if ((alminib_table[i].p_cntcb->p_cntinib != NULL) &&
			(alminib_table[i].p_cntcb->p_cntinib->coreid == coreid)) {
			p_almcb = p_almcb_table[i];

			p_almcb->p_alminib = &(alminib_table[i]);
			(p_almcb->cntexpinfo).expirefunc = &alarm_expire;
			if ((p_almcb->p_alminib->autosta & ((AppModeType) 1 << appmodeid)) != APPMODE_NONE) {
				if (p_almcb->p_alminib->actatr == ABSOLUTE) {
					/*
					 *  絶対時間の起動
					 *  満了時間が0の場合，次の周期の0のタイミングとなる
					 *  （get_abstickに考慮済み）
					 */
					(p_almcb->cntexpinfo).expiretick =
						get_abstick(p_almcb->p_alminib->p_cntcb, p_almcb->p_alminib->almval);
				}
				else {
					/* 相対時間の起動 */
					(p_almcb->cntexpinfo).expiretick =
						get_reltick(p_almcb->p_alminib->p_cntcb, p_almcb->p_alminib->almval);
				}
				p_almcb->cycle = p_almcb->p_alminib->cycle;

				insert_cnt_expr_que(&(p_almcb->cntexpinfo), p_almcb->p_alminib->p_cntcb);
			}
			else {
				queue_initialize(&(p_almcb->cntexpinfo.cntexpque));
			}
		}
	}
}

#endif /* TOPPERS_alarm_initialize */

/*
 *  アラーム情報の取得
 */
#ifdef TOPPERS_GetAlarmBase

StatusType
GetAlarmBase(AlarmType AlarmID, AlarmBaseRefType Info)
{
	StatusType	ercd = E_OK;
	ALMCB		*p_almcb;
	CNTCB		*p_cntcb;
	OSAPCB		*p_osapcb;
	CCB			*p_ccb;

	LOG_GETALB_ENTER(AlarmID);
	CHECK_DISABLEDINT();
	CHECK_CALLEVEL(CALLEVEL_GETALARMBASE);
	CHECK_ID(AlarmID < tnum_alarm);
	CHECK_PARAM_POINTER(Info);
	CHECK_MEM_WRITE(Info, AlarmBaseType);
	p_almcb = get_almcb(AlarmID);
	CHECK_CORE(p_almcb->p_alminib != NULL);
	CHECK_RIGHT(p_almcb->p_alminib->acsbtmp);

	p_cntcb = p_almcb->p_alminib->p_cntcb;
	p_osapcb = p_almcb->p_alminib->p_osapcb;
	p_ccb = get_p_ccb(p_cntcb->p_cntinib->coreid);
	x_nested_lock_os_int();
	acquire_cnt_lock(p_ccb);

	/* アラーム所属のOSAPの状態をチェック */
	D_CHECK_ACCESS((p_osapcb->osap_stat == APPLICATION_ACCESSIBLE) ||
				   ((p_osapcb->osap_stat == APPLICATION_RESTARTING) &&
					(p_osapcb == get_my_p_ccb()->p_runosap)));

	Info->maxallowedvalue = p_cntcb->p_cntinib->maxval;
	Info->ticksperbase = p_cntcb->p_cntinib->tickbase;
	Info->mincycle = p_cntcb->p_cntinib->mincyc;

	release_cnt_lock(p_ccb);
	x_nested_unlock_os_int();

  exit_no_errorhook:
	LOG_GETALB_LEAVE(ercd, Info);
	return(ercd);

#ifdef CFG_USE_ERRORHOOK
  d_exit_errorhook:
	release_cnt_lock(p_ccb);
	goto errorhook_start;

  exit_errorhook:
	x_nested_lock_os_int();
  errorhook_start:
#ifdef CFG_USE_PARAMETERACCESS
	get_my_p_ccb()->_errorhook_par1.almid = AlarmID;
	get_my_p_ccb()->_errorhook_par2.p_info = Info;
#endif /* CFG_USE_PARAMETERACCESS */
	call_errorhook(ercd, OSServiceId_GetAlarmBase);
#else /* CFG_USE_ERRORHOOK */
  d_exit_no_errorhook:
	release_cnt_lock(p_ccb);
#endif /* CFG_USE_ERRORHOOK */
	x_nested_unlock_os_int();
	goto exit_no_errorhook;
}

#endif /* TOPPERS_GetAlarmBase */

/*
 *  アラームの状態参照
 */
#ifdef TOPPERS_GetAlarm

StatusType
GetAlarm(AlarmType AlarmID, TickRefType Tick)
{
	StatusType	ercd = E_OK;
	TickType	curval;
	ALMCB		*p_almcb;
	CNTCB		*p_cntcb;
	OSAPCB		*p_osapcb;
	CCB			*p_ccb;

	LOG_GETALM_ENTER(AlarmID);
	CHECK_DISABLEDINT();
	CHECK_CALLEVEL(CALLEVEL_GETALARM);
	CHECK_ID(AlarmID < tnum_alarm);
	CHECK_PARAM_POINTER(Tick);
	CHECK_MEM_WRITE(Tick, TickType);
	p_almcb = get_almcb(AlarmID);
	CHECK_CORE(p_almcb->p_alminib != NULL);
	CHECK_RIGHT(p_almcb->p_alminib->acsbtmp);

	p_cntcb = p_almcb->p_alminib->p_cntcb;
	p_osapcb = p_almcb->p_alminib->p_osapcb;
	p_ccb = get_p_ccb(p_cntcb->p_cntinib->coreid);
	x_nested_lock_os_int();
	acquire_cnt_lock(p_ccb);
	/* アラーム所属のOSAPの状態をチェック */
	D_CHECK_ACCESS((p_osapcb->osap_stat == APPLICATION_ACCESSIBLE) ||
				   ((p_osapcb->osap_stat == APPLICATION_RESTARTING) &&
					(p_osapcb == get_my_p_ccb()->p_runosap)));

	S_D_CHECK_NOFUNC(queue_empty(&(p_almcb->cntexpinfo.cntexpque)) == FALSE);

	/*
	 *  カウンタの現在値を取得
	 *  ハードウェアカウンタの場合，既に満了している可能性がある
	 */
	curval = get_curval(p_cntcb, CNTID(p_cntcb));

	*Tick = diff_tick(p_almcb->cntexpinfo.expiretick, curval, p_cntcb->p_cntinib->maxval2);

  d_exit_no_errorhook:
	release_cnt_lock(p_ccb);
	x_nested_unlock_os_int();
  exit_no_errorhook:
	LOG_GETALM_LEAVE(ercd, Tick);
	return(ercd);

#ifdef CFG_USE_ERRORHOOK
  d_exit_errorhook:
	release_cnt_lock(p_ccb);
	goto errorhook_start;

  exit_errorhook:
	x_nested_lock_os_int();
  errorhook_start:
#ifdef CFG_USE_PARAMETERACCESS
	get_my_p_ccb()->_errorhook_par1.almid = AlarmID;
	get_my_p_ccb()->_errorhook_par2.p_tick = Tick;
#endif /* CFG_USE_PARAMETERACCESS */
	call_errorhook(ercd, OSServiceId_GetAlarm);
	x_nested_unlock_os_int();
	goto exit_no_errorhook;
#endif /* CFG_USE_ERRORHOOK */
}

#endif /* TOPPERS_GetAlarm */

/*
 *  アラームの設定（相対値）
 */
#ifdef TOPPERS_SetRelAlarm

StatusType
SetRelAlarm(AlarmType AlarmID, TickType increment, TickType cycle)
{
	StatusType	ercd = E_OK;
	TickType	maxval;
	ALMCB		*p_almcb;
	CNTCB		*p_cntcb;
	OSAPCB		*p_osapcb;
	CCB			*p_ccb;

	LOG_SETREL_ENTER(AlarmID, increment, cycle);
	CHECK_DISABLEDINT();
	CHECK_CALLEVEL(CALLEVEL_SETRELALARM);
	CHECK_ID(AlarmID < tnum_alarm);
	p_almcb = get_almcb(AlarmID);
	CHECK_CORE(p_almcb->p_alminib != NULL);
	CHECK_RIGHT(p_almcb->p_alminib->acsbtmp);
	p_cntcb = p_almcb->p_alminib->p_cntcb;

	maxval = p_cntcb->p_cntinib->maxval;
	CHECK_VALUE((0U < increment) && (increment <= maxval));
	CHECK_VALUE((cycle == 0U)
				|| ((p_cntcb->p_cntinib->mincyc <= cycle) && (cycle <= maxval)));

	p_osapcb = p_almcb->p_alminib->p_osapcb;
	p_ccb = get_p_ccb(p_cntcb->p_cntinib->coreid);
	x_nested_lock_os_int();
	acquire_cnt_lock(p_ccb);
	/* 起動するアラーム所属のOSAPの状態をチェック */
	D_CHECK_ACCESS((p_osapcb->osap_stat == APPLICATION_ACCESSIBLE) ||
				   ((p_osapcb->osap_stat == APPLICATION_RESTARTING) &&
					(p_osapcb == get_my_p_ccb()->p_runosap)));

	S_D_CHECK_STATE(queue_empty(&(p_almcb->cntexpinfo.cntexpque)) != FALSE);

	p_almcb->cntexpinfo.expiretick = get_reltick(p_cntcb, increment);
	p_almcb->cycle = cycle;

	insert_cnt_expr_que(&(p_almcb->cntexpinfo), p_cntcb);

  d_exit_no_errorhook:
	release_cnt_lock(p_ccb);
	x_nested_unlock_os_int();
  exit_no_errorhook:
	LOG_SETREL_LEAVE(ercd);
	return(ercd);

#ifdef CFG_USE_ERRORHOOK
  d_exit_errorhook:
	release_cnt_lock(p_ccb);
	goto errorhook_start;

  exit_errorhook:
	x_nested_lock_os_int();
  errorhook_start:
#ifdef CFG_USE_PARAMETERACCESS
	get_my_p_ccb()->_errorhook_par1.almid = AlarmID;
	get_my_p_ccb()->_errorhook_par2.incr = increment;
	get_my_p_ccb()->_errorhook_par3.cycle = cycle;
#endif /* CFG_USE_PARAMETERACCESS */
	call_errorhook(ercd, OSServiceId_SetRelAlarm);
	x_nested_unlock_os_int();
	goto exit_no_errorhook;
#endif /* CFG_USE_ERRORHOOK */
}

#endif /* TOPPERS_SetRelAlarm */

/*
 *  アラームの設定（絶対値）
 */
#ifdef TOPPERS_SetAbsAlarm

StatusType
SetAbsAlarm(AlarmType AlarmID, TickType start, TickType cycle)
{
	StatusType	ercd = E_OK;
	TickType	maxval;
	ALMCB		*p_almcb;
	CNTCB		*p_cntcb;
	OSAPCB		*p_osapcb;
	CCB			*p_ccb;

	LOG_SETABS_ENTER(AlarmID, start, cycle);
	CHECK_DISABLEDINT();
	CHECK_CALLEVEL(CALLEVEL_SETABSALARM);
	CHECK_ID(AlarmID < tnum_alarm);
	p_almcb = get_almcb(AlarmID);
	CHECK_CORE(p_almcb->p_alminib != NULL);
	CHECK_RIGHT(p_almcb->p_alminib->acsbtmp);
	p_cntcb = p_almcb->p_alminib->p_cntcb;

	maxval = p_cntcb->p_cntinib->maxval;
	CHECK_VALUE(start <= maxval);
	CHECK_VALUE((cycle == 0U)
				|| ((p_cntcb->p_cntinib->mincyc <= cycle) && (cycle <= maxval)));

	p_osapcb = p_almcb->p_alminib->p_osapcb;
	p_ccb = get_p_ccb(p_cntcb->p_cntinib->coreid);
	x_nested_lock_os_int();
	acquire_cnt_lock(p_ccb);
	/* 起動するアラーム所属のOSAPの状態をチェック */
	D_CHECK_ACCESS((p_osapcb->osap_stat == APPLICATION_ACCESSIBLE) ||
				   ((p_osapcb->osap_stat == APPLICATION_RESTARTING) &&
					(p_osapcb == get_my_p_ccb()->p_runosap)));

	S_D_CHECK_STATE(queue_empty(&(p_almcb->cntexpinfo.cntexpque)) != FALSE);

	p_almcb->cntexpinfo.expiretick = get_abstick(p_cntcb, start);
	p_almcb->cycle = cycle;

	insert_cnt_expr_que(&(p_almcb->cntexpinfo), p_cntcb);

  d_exit_no_errorhook:
	release_cnt_lock(p_ccb);
	x_nested_unlock_os_int();
  exit_no_errorhook:
	LOG_SETABS_LEAVE(ercd);
	return(ercd);

#ifdef CFG_USE_ERRORHOOK
  d_exit_errorhook:
	release_cnt_lock(p_ccb);
	goto errorhook_start;

  exit_errorhook:
	x_nested_lock_os_int();
  errorhook_start:
#ifdef CFG_USE_PARAMETERACCESS
	get_my_p_ccb()->_errorhook_par1.almid = AlarmID;
	get_my_p_ccb()->_errorhook_par2.start = start;
	get_my_p_ccb()->_errorhook_par3.cycle = cycle;
#endif /* CFG_USE_PARAMETERACCESS */
	call_errorhook(ercd, OSServiceId_SetAbsAlarm);
	x_nested_unlock_os_int();
	goto exit_no_errorhook;
#endif /* CFG_USE_ERRORHOOK */
}

#endif /* TOPPERS_SetAbsAlarm */

/*
 *  アラームのキャンセル
 */
#ifdef TOPPERS_CancelAlarm

StatusType
CancelAlarm(AlarmType AlarmID)
{
	StatusType	ercd = E_OK;
	ALMCB		*p_almcb;
	CNTCB		*p_cntcb;
	OSAPCB		*p_osapcb;
	CCB			*p_ccb;

	LOG_CANALM_ENTER(AlarmID);
	CHECK_DISABLEDINT();
	CHECK_CALLEVEL(CALLEVEL_CANCELALARM);
	CHECK_ID(AlarmID < tnum_alarm);
	p_almcb = get_almcb(AlarmID);
	CHECK_CORE(p_almcb->p_alminib != NULL);
	CHECK_RIGHT(p_almcb->p_alminib->acsbtmp);

	p_cntcb = p_almcb->p_alminib->p_cntcb;
	p_osapcb = p_almcb->p_alminib->p_osapcb;
	p_ccb = get_p_ccb(p_cntcb->p_cntinib->coreid);
	x_nested_lock_os_int();
	acquire_cnt_lock(p_ccb);
	/* キャンセルするアラーム所属のOSAPの状態をチェック */
	D_CHECK_ACCESS((p_osapcb->osap_stat == APPLICATION_ACCESSIBLE) ||
				   ((p_osapcb->osap_stat == APPLICATION_RESTARTING) &&
					(p_osapcb == get_my_p_ccb()->p_runosap)));

	S_D_CHECK_NOFUNC(queue_empty(&(p_almcb->cntexpinfo.cntexpque)) == FALSE);

	delete_cnt_expr_que(&(p_almcb->cntexpinfo), p_cntcb);

  d_exit_no_errorhook:
	release_cnt_lock(p_ccb);
	x_nested_unlock_os_int();
  exit_no_errorhook:
	LOG_CANALM_LEAVE(ercd);
	return(ercd);

#ifdef CFG_USE_ERRORHOOK
  d_exit_errorhook:
	release_cnt_lock(p_ccb);
	goto errorhook_start;

  exit_errorhook:
	x_nested_lock_os_int();
  errorhook_start:
#ifdef CFG_USE_PARAMETERACCESS
	get_my_p_ccb()->_errorhook_par1.almid = AlarmID;
#endif /* CFG_USE_PARAMETERACCESS */
	call_errorhook(ercd, OSServiceId_CancelAlarm);
	x_nested_unlock_os_int();
	goto exit_no_errorhook;
#endif /* CFG_USE_ERRORHOOK */
}

#endif /* TOPPERS_CancelAlarm */

/*
 *  アラーム満了アクション処理用関数
 */
#ifdef TOPPERS_alarm_expire

void
alarm_expire(CNTEXPINFO *p_cntexpinfo, const CNTCB *p_cntcb)
{
	ALMCB	*p_almcb;
	OSAPCB	*p_runosap_saved;
	CCB		*p_ccb = get_my_p_ccb();

	p_almcb = (ALMCB *) p_cntexpinfo;

	/* カウンタ満了キューへの再挿入（周期アラームの場合） */
	if (p_almcb->cycle > 0U) {
		p_cntexpinfo->expiretick = add_tick(p_cntexpinfo->expiretick, p_almcb->cycle,
											p_cntcb->p_cntinib->maxval2);
		insert_cnt_expr_que(p_cntexpinfo, (CNTCB *) p_cntcb);
	}

	release_cnt_lock(p_ccb);
	/* アラーム満了アクションの呼出し */
	LOG_ALM_ENTER(p_almcb);

	p_runosap_saved = p_ccb->p_runosap;
	p_ccb->p_runosap = p_almcb->p_alminib->p_osapcb;
	(p_almcb->p_alminib->action)();
	p_ccb->p_runosap = p_runosap_saved;

	LOG_ALM_LEAVE(p_almcb);
	acquire_cnt_lock(p_ccb);
}

#endif /* TOPPERS_alarm_expire */

/*
 *  OSAP所属するアラームの強制終了
 */
#ifdef TOPPERS_force_term_osap_alarm

void
force_term_osap_alarm(OSAPCB *p_osapcb)
{
	/* カウンタの満了キューから消す */
	AlarmType	i;
	ALMCB		*p_almcb;

	for (i = 0U; i < tnum_alarm; i++) {
		if (alminib_table[i].p_osapcb == p_osapcb) {
			p_almcb = p_almcb_table[i];
			/*
			 *  満了キューから削除する
			 *  初期化より先に実施する必要がある
			 */
			delete_cnt_expr_que(&(p_almcb->cntexpinfo), p_almcb->p_alminib->p_cntcb);
			p_almcb->cycle = 0U;
		}
	}
}

#endif /* TOPPERS_force_term_osap_alarm */
