#include "TN_SignalState.h"

TN_Signal::TN_Signal() {
  Init();
}


void TN_Signal::Init()
{
    tn_state_ = Idring();
}

TnSignalState TN_Signal::GiveSignal(boolean is_high)
{
    /* チャタリング対策の簡易的なローパスフィルタ */
    if(millis() - receipt_time_ > chatter_time_){
        tn_state_->GiveSignal(is_high, this);
        receipt_time_ = millis();
        return tn_state_->GetStateEnum();
    }
    return TnSignalState::kLowPassFiltering;
}


/* ---------------------------------------------------------------------- */

TN_State *TN_Signal::Idring() const
{
    static IdringState state;
    return &state;
}

TN_State *TN_Signal::FirstPress() const
{
    static FirstPressState state;
    return &state;
}

TN_State *TN_Signal::FirstHold() const
{
    static FirstHoldState state;
    return &state;
}

TN_State *TN_Signal::FirstRelease() const
{
    static FirstReleaseState state;
    return &state;
}

TN_State *TN_Signal::SecondIdring() const
{
    static SecondIdringState state;
    return &state;
}

TN_State *TN_Signal::SecondPress() const
{
    static SecondPressState state;
    return &state;
}

TN_State *TN_Signal::SecondHold() const
{
    static SecondHoldState state;
    return &state;
}

TN_State *TN_Signal::SecondRelease() const
{
    static SecondReleaseState state;
    return &state;
}

TN_State *TN_Signal::ThirdIdring() const
{
    static ThirdIdringState state;
    return &state;
}

TN_State *TN_Signal::LongPress() const
{
    static LongPressState state;
    return &state;
}

TN_State *TN_Signal::LongHold() const
{
    static LongHoldState state;
    return &state;
}

TN_State *TN_Signal::LongRelease() const
{
    static LongReleaseState state;
    return &state;
}


/* ---------------------------------------------------------------------- */

TN_State::TN_State() {}
TN_State::~TN_State() {}

/* ---------------------------------------------------------------------- */

/* 待機状態(default state) */
void IdringState::GiveSignal(boolean is_high, TN_Signal *ctx)
{
    if (is_high)
    {
        ctx->set_tn_state(ctx->FirstPress()); /* 状態遷移 */
    }
}

/* 1回目のシグナルが立ち上がった状態 */
void FirstPressState::GiveSignal(boolean is_high, TN_Signal *ctx)
{
    if (is_high)
    {
        ctx->set_start_time(millis());        /* start timing measurement */
        ctx->set_tn_state(ctx->FirstHold()); /* 状態遷移 */
    }
    else
    {
        ctx->set_tn_state(ctx->FirstRelease()); /* 状態遷移 */
    }
}

/* 1回目のシグナルが立ち続けている状態 */
void FirstHoldState::GiveSignal(boolean is_high, TN_Signal *ctx)
{
    uint32_t start = ctx->start_time();
    uint32_t long_press = ctx->long_press_time();

    if (is_high)
    {
        if (millis() - start > long_press)
        {
            ctx->set_tn_state(ctx->LongPress()); /* 状態遷移 */
        }
    }
    else
    {
        ctx->set_tn_state(ctx->FirstRelease()); /* 状態遷移 */
    }
}

/* 1回目のシグナルが落ちた状態 */
void FirstReleaseState::GiveSignal(boolean is_high, TN_Signal *ctx)
{
    if (is_high)
    {
        ctx->set_tn_state(ctx->SecondPress()); /* 状態遷移 */
    }
    else
    {
        ctx->set_start_time(millis());          /* タイマーのリセット */
        ctx->set_tn_state(ctx->SecondIdring()); /* 状態遷移 */
    }
}

/* 2回目のシグナルの待機状態 */
void SecondIdringState::GiveSignal(boolean is_high, TN_Signal *ctx)
{
    uint32_t start = ctx->start_time();
    uint32_t double_click = ctx->second_press_time();

    if (is_high)
    {
        ctx->set_tn_state(ctx->SecondPress()); /* 状態遷移 */
    }
    else
    {
        if (millis() - start > double_click)
        {
            ctx->set_tn_state(ctx->Idring()); /* 状態遷移 */
        }
    }
}

/* 2回目のシグナルが立ち上がった状態 */
void SecondPressState::GiveSignal(boolean is_high, TN_Signal *ctx)
{
    if (is_high)
    {
        ctx->set_start_time(millis());        /* タイマーのリセット */
        ctx->set_tn_state(ctx->SecondHold()); /* 状態遷移 */
    }
    else
    {
        ctx->set_tn_state(ctx->SecondRelease()); /* 状態遷移 */
    }
}

/* 2回目のシグナルが立ち続けている状態 */
void SecondHoldState::GiveSignal(boolean is_high, TN_Signal *ctx)
{
    uint32_t start = ctx->start_time();
    uint32_t long_press = ctx->long_press_time();

    if (is_high)
    {
        if (millis() - start > long_press)
        {
            ctx->set_tn_state(ctx->LongPress()); /* 状態遷移 */
        }
    }
    else
    {
        ctx->set_tn_state(ctx->SecondRelease()); /* 状態遷移 */
    }
}

/* 2回目のシグナルが落ちた状態 */
void SecondReleaseState::GiveSignal(boolean is_high, TN_Signal *ctx)
{
    uint32_t start = ctx->start_time();
    uint32_t long_press = ctx->long_press_time();

    if (is_high)
    {
        ctx->set_tn_state(ctx->SecondPress()); /* 状態遷移 */
    }
    else
    {
        // ctx->set_tn_state(ctx->Idring()); /* 状態遷移 */
        ctx->set_start_time(millis());          /* タイマーのリセット */
        ctx->set_tn_state(ctx->ThirdIdring()); /* 状態遷移 */
    }
}

/* 3回目のシグナルの待機状態 */
void ThirdIdringState::GiveSignal(boolean is_high, TN_Signal *ctx)
{
    uint32_t start = ctx->start_time();
    uint32_t double_click = ctx->second_press_time();

    if (is_high)
    {
        ctx->set_tn_state(ctx->SecondPress()); /* 状態遷移 */
    }
    else
    {
        if (millis() - start > double_click)
        {
            ctx->set_tn_state(ctx->Idring()); /* 状態遷移 */
        }
    }
}

/* 長時間シグナルが立ち続けてた状態 */
void LongPressState::GiveSignal(boolean is_high, TN_Signal *ctx)
{
    if (is_high)
    {
        ctx->set_tn_state(ctx->LongHold()); /* 状態遷移 */
    }
    else
    {
        ctx->set_tn_state(ctx->LongRelease()); /* 状態遷移 */
    }
}

/* 長時間シグナルが立ち続けている状態 */
void LongHoldState::GiveSignal(boolean is_high, TN_Signal *ctx)
{
    if (is_high)
    {
        // nothing
    }
    else
    {
        ctx->set_tn_state(ctx->LongRelease()); /* 状態遷移 */
    }
}

/* 長時間シグナルが立った後、シグナルが落ちた状態 */
void LongReleaseState::GiveSignal(boolean is_high, TN_Signal *ctx)
{

    if (is_high)
    {
        ctx->set_tn_state(ctx->FirstPress()); /* 状態遷移 */
    }
    else
    {
        ctx->set_tn_state(ctx->Idring()); /* 状態遷移 */
    }
}
