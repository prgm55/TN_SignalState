

#include "TN_SignalState.h"
TN_Signal::TN_Signal() {
  Init();
}


//TN_Signal::TN_Signal() : TN_Signal::TN_Signal(false) {}
//
//TN_Signal::TN_Signal(boolean is_toggle) : is_toggle_{is_toggle}
//{
//    Init();
//}

void TN_Signal::Init()
{
    tn_state_ = Idring();
    // if (is_toggle_)
    // {
    //     tn_state_ = ToggleOff();
    // }
    // else
    // {
    //     tn_state_ = Idring();
    // }

}

/**
 * 
**/
TnSignalState TN_Signal::GiveSignal(boolean is_high)
{
    /* Low-pass filter to prevent chattering */
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

TN_State *TN_Signal::SinglePress() const
{
    static SinglePressState state;
    return &state;
}

TN_State *TN_Signal::SingleHold() const
{
    static SingleHoldState state;
    return &state;
}

TN_State *TN_Signal::SingleRelease() const
{
    static SingleReleaseState state;
    return &state;
}

TN_State *TN_Signal::DoubleIdring() const
{
    static DoubleIdringState state;
    return &state;
}

TN_State *TN_Signal::DoublePress() const
{
    static DoublePressState state;
    return &state;
}

TN_State *TN_Signal::DoubleHold() const
{
    static DoubleHoldState state;
    return &state;
}

TN_State *TN_Signal::DoubleRelease() const
{
    static DoubleReleaseState state;
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
        ctx->set_tn_state(ctx->SinglePress()); /* 状態遷移 */
    }
}

/* 1回目のシグナルが立ち上がった状態 */
void SinglePressState::GiveSignal(boolean is_high, TN_Signal *ctx)
{
    if (is_high)
    {
        ctx->set_start_time(millis());        /* start timing measurement */
        ctx->set_tn_state(ctx->SingleHold()); /* 状態遷移 */
    }
    else
    {
        ctx->set_tn_state(ctx->SingleRelease()); /* 状態遷移 */
    }
}

/* 1回目のシグナルが立ち続けている状態 */
void SingleHoldState::GiveSignal(boolean is_high, TN_Signal *ctx)
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
        ctx->set_tn_state(ctx->SingleRelease()); /* 状態遷移 */
    }
}

/* 1回目のシグナルが落ちた状態 */
void SingleReleaseState::GiveSignal(boolean is_high, TN_Signal *ctx)
{
    if (is_high)
    {
        ctx->set_tn_state(ctx->DoublePress()); /* 状態遷移 */
    }
    else
    {
        ctx->set_start_time(millis());          /* タイマーのリセット */
        ctx->set_tn_state(ctx->DoubleIdring()); /* 状態遷移 */
    }
}

/* 2回目のシグナルの待機状態 */
void DoubleIdringState::GiveSignal(boolean is_high, TN_Signal *ctx)
{
    uint32_t start = ctx->start_time();
    uint32_t double_click = ctx->double_press_time();

    if (is_high)
    {
        ctx->set_tn_state(ctx->DoublePress()); /* 状態遷移 */
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
void DoublePressState::GiveSignal(boolean is_high, TN_Signal *ctx)
{
    if (is_high)
    {
        ctx->set_start_time(millis());        /* タイマーのリセット */
        ctx->set_tn_state(ctx->DoubleHold()); /* 状態遷移 */
    }
    else
    {
        ctx->set_tn_state(ctx->DoubleRelease()); /* 状態遷移 */
    }
}

/* 2回目のシグナルが立ち続けている状態 */
void DoubleHoldState::GiveSignal(boolean is_high, TN_Signal *ctx)
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
        ctx->set_tn_state(ctx->DoubleRelease()); /* 状態遷移 */
    }
}

/* 2回目のシグナルが落ちた状態 */
void DoubleReleaseState::GiveSignal(boolean is_high, TN_Signal *ctx)
{
    uint32_t start = ctx->start_time();
    uint32_t long_press = ctx->long_press_time();

    if (is_high)
    {
        ctx->set_tn_state(ctx->DoublePress()); /* 状態遷移 */
    }
    else
    {
        // ctx->set_tn_state(ctx->Idring()); /* 状態遷移 */
        ctx->set_tn_state(ctx->DoubleIdring()); /* 状態遷移 */
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
        ctx->set_tn_state(ctx->SinglePress()); /* 状態遷移 */
    }
    else
    {
        ctx->set_tn_state(ctx->Idring()); /* 状態遷移 */
    }
}
