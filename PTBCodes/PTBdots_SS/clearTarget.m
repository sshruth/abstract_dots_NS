function clearTarget(x, y, x2, y2)% This function clears targetsglobal TARGET_SIZE_PIX APBG_INDEXglobal CUR_WINDOW CENTER_X CENTER_Yglobal PIX_PER_DEGe=TARGET_SIZE_PIX/2 +2;wx = x * PIX_PER_DEG /10 + CENTER_X;wy = -y * PIX_PER_DEG /10 + CENTER_Y;wx2 = x2 * PIX_PER_DEG /10 + CENTER_X;wy2 = -y2 * PIX_PER_DEG /10 + CENTER_Y;Screen('FillOval',CUR_WINDOW,APBG_INDEX,[wx-e wy-e wx+e wy+e]);fprintf('(t1 off)');Screen('FillOval',CUR_WINDOW,APBG_INDEX,[wx2-e wy2-e wx2+e wy2+e]);fprintf('(t2 off)');Screen('Flip',CUR_WINDOW);