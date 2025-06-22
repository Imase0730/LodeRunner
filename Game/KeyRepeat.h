#pragma once

// 簡易キーリピートクラス
class KeyRepeat
{
private:

	// キーリピートの間隔
	static constexpr int KEY_REPEAT_INTEVAL = 10;

public:

	// 簡易キーリピート
	static int GetKey(int keyCondition)
	{
		// キーが押されたフレーム数
		static int elapseFrame = 0;
		// １フレーム前のキー情報
		static int oldKey = 0;

		int keyTrigger = ~oldKey & keyCondition;
		int keyRepeat = 0;

		// キーが押されている
		if (keyCondition)
		{
			// キーが押された瞬間はキー情報を反映する
			if (keyTrigger)
			{
				keyRepeat = keyCondition;
				elapseFrame = 0;
			}
			// リピート間隔を超えた
			if (++elapseFrame >= KEY_REPEAT_INTEVAL)
			{
				elapseFrame = 0;
				keyRepeat = keyCondition;
			}
		}

		oldKey = keyCondition;

		return keyRepeat;
	}
};