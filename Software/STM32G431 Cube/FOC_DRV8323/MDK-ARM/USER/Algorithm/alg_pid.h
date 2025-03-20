/*
 * @Project: Infantry Code
 *
 * @Author: GDDG08
 * @Date: 2021-12-31 17:37:14
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2024-04-19 23:25:35
 */

#ifndef ALG_PID_H
#define ALG_PID_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "lib_math.h"
#include "lib_filter.h"

    typedef enum
    {
        PID_POSITION = 0u,
        PID_DELTA = 1u
    } PID_ModeEnum;

    typedef struct
    {
        PID_ModeEnum pid_mode;
        float kp;
        float ki;
        float kd;
        float kf_1;
        float kf_2;
        float sum_max;
        float output_max;
        Filter_Lowpass_TypeDef d_filter;
        Filter_Lowpass_TypeDef kf1_filter;
        Filter_Lowpass_TypeDef kf2_filter;

        float ref;
        float fdb;
        float sum;
        float output;

        float err[3];
        float err_fdf[3]; // Feedforard
        float err_lim;    // Integral anti-windup
        float output_fdf; // Feedforard output
        float err_watch;
    } PID_TypeDef;

    typedef enum
    {
        NB = 0,
        NM = 1,
        NS = 2,
        ZO = 3,
        PS = 4,
        PM = 5,
        PB = 6,
    } FuzzyPID_TableEnum;

    typedef struct
    {
        float left;
        float right;
    } Interval;

    typedef struct
    {
        float kp;
        float ki;
        float kd;

        const uint8_t (*kp_rule)[7];
        const uint8_t (*ki_rule)[7];
        const uint8_t (*kd_rule)[7];

        float *kp_set;
        float *ki_set;
        float *kd_set;
        float sum_max;
        float output_max;
        Interval error_range;
        Interval error_change_range;
        Filter_Lowpass_TypeDef p_filter;
        Filter_Lowpass_TypeDef d_filter;

        float ref;
        float fdb;
        float sum;
        float output;
        float error;
        float error_last;
    } FuzzyPID_TypeDef;

    extern uint8_t Kp_RuleList[7][7];
    extern uint8_t Ki_RuleList[7][7];
    extern uint8_t Kd_RuleList[7][7];

    void PID_Init(PID_TypeDef *pid, PID_ModeEnum mode,
                  float kp, float ki, float kd, float kf_1, float kf_2,
                  float sum_max, float output_max, float kd_filter, float kf1_filter, float kf2_filter);
    void PID_Clear(PID_TypeDef *pid);
    float PID_Calc(PID_TypeDef *pid);
    float PID_GetRef(PID_TypeDef *pid);
    void PID_SetRef(PID_TypeDef *pid, float ref);
    void PID_AddRef(PID_TypeDef *pid, float inc);
    float PID_GetFdb(PID_TypeDef *pid);
    void PID_SetFdb(PID_TypeDef *pid, float fdb);
    float PID_GetOutput(PID_TypeDef *pid);

    void FuzzyPID_Init(FuzzyPID_TypeDef *fuzzy_pid,
                       const uint8_t (*kp_rule)[7], const uint8_t (*ki_rule)[7], const uint8_t (*kd_rule)[7],
                       float kp_set[7], float ki_set[7], float kd_set[7],
                       Interval *error_range, Interval *error_change_range,
                       float sum_max, float output_max,
                       float p_filter_param, float d_filter_param);
    float FuzzyPID_Calc(FuzzyPID_TypeDef *fuzzy_pid);
    void FuzzyPID_SetRef(FuzzyPID_TypeDef *fuzzy_pid, float ref);
    void FuzzyPID_SetFdb(FuzzyPID_TypeDef *fuzzy_pid, float fdb);
    float TableLookup(const uint8_t (*rule)[7], float *set, Interval *eRange, Interval *ecRange, float e, float ec);

#ifdef __cplusplus
}
#endif

#endif
