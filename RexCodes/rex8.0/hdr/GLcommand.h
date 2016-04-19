/* this definition is for those who want to use
 *rex command format instead of vex actions
 */
extern unsigned char vexbuf[16384];
extern long vx;
extern long vy;

/*
 * Commands for VEX version GLvex
 */
#define SET_ERASE_METHOD				255
#define ENABLE_REX_VIDEO_SYNC			254
#define DISABLE_REX_VIDEO_SYNC			253
#define ENABLE_REX_DIGITAL_SYNC			252
#define DISABLE_REX_DIGITAL_SYNC		251
#define SET_REX_SCALE					250
#define SET_BACK_LUM					249
#define SET_FP_ON_CLR					248
#define SET_FP_DIM_CLR					247
#define SET_FP_SIZE						246
#define SET_STIM_LUMINANCES				245
#define SET_STIM_COLORS					244
#define SET_GRAY_SCALE					243
#define SET_OBJECT_GRAY_SCALE			242
#define SET_LUT_ENTRY_CLR				241
#define SET_OBJECT_LUT_ENTRY_CLR		240
#define ALL_OFF					   		239
#define SWITCH_FIX_POINT				238
#define DIM_FIX_POINT					237
#define PRELOAD_STIM					236
#define SWAP_BUFFERS					235
#define SWITCH_STIM						234
#define SET_STIM_SWITCH					233
#define TIME_STIM						232
#define SEQUENCE_STIM					231
#define SET_FP_LOCATION					230
#define STIM_LOCATION					229
#define STIM_FROM_FIX_POINT				228
#define SHIFT_LOCATION					227
#define REPORT_LOCATION					226
#define SET_ACTIVE_OBJECT				225
#define CLIP_RECT_SET					224
#define CLIP_RECT_SET_FROM_FP			223
#define FULL_CLIP_RECT					222
#define DRAW_WALSH_PATTERN				221
#define DRAW_HAAR_PATTERN				220
#define DRAW_RANDOM_PATTERN				219
#define DRAW_ANNULUS					218
#define DRAW_BAR						217
#define DRAW_FLOW_PATTERN				216
#define MASK_FLOW						215
#define DRAW_USER_PATTERN				214
#define DRAW_RGB_USER_PATTERN			213
#define DRAW_TIFF_IMAGE					212
#define DRAW_OKN_PATTERN				211
#define LOAD_PATTERN					210
#define COPY_OBJECT						209
#define NEW_RAMP						208
#define NEW_RAMP_FROM_FP				207
#define LOAD_RAMP						206
#define LOAD_PIXEL_RAMP					205
#define TO_RAMP_START					204
#define START_RAMP						203
#define RESET_RAMPS						202
#define NEW_FLOW						201
#define START_FLOW						200
#define TIME_FLOW						199
#define MAKE_FLOW_MOVIE					198
#define TO_FLOW_MOVIE_START				197
#define SHOW_FLOW_MOVIE					196
#define START_FLOW_RAMP					195
#define STOP_FLOW_RAMP					194
#define START_OKN						193
#define STOP_OKN						192
#define SHOW_MOVIE_CLIP					191
#define STOP_MOVIE						190
#define SET_TRIGGERS					189
#define ENABLE_REX_FRAME_SYNC			188
#define DISABLE_REX_FRAME_SYNC			187
#define DRAW_BIG_ANNULUS				186
#define LOAD_REX_POINT_ARRAY			185
#define SHIFT_FLOW						184
#define DRAW_ELLIPTICAL_FLOW_PATTERN	183
#define SET_COLOR_MASK					182
#define SET_ALIASING					181
#define ROTATE_OBJECT					180
#define DRAW_BIG_BAR                    179
#define REX_KEYBOARD                    178
#define REX_FUNCTION_KEY                177
#define REX_MOTION                      176
#define REX_MOUSE                       175
#define RANDOM_DIRECTION_LIMIT			174
#define RANDOM_SPEED_LIMIT				173
#define HELLO                           172
#define GOODBYE                         171
#define DRAW_SINE_WAVE					170
#define DRAW_SQUARE_WAVE				169
#define START_SINE						168
#define TIME_GRATING					167
#define START_SINE_RAMP					166
#define STOP_SINE						165
#define SET_TRANSPARENCY				164
#define	SET_REX_VIDEO_SYNC				163
#define DRAW_HALF_SINE_WAVE				162
#define DRAW_HALF_SQUARE_WAVE			161
#define LOAD_SOUND						160
#define DRAW_PNG_IMAGE					159

/*
 * Vex return signals
 */
#define ABORT_TRIAL			255
#define FIX_ON				254
#define FIX_OFF				253
#define FIX_DIM				252
#define STIMULUS_ON			251
#define STIMULUS_OFF		250
#define LIST_ON				249
#define LIST_OFF			248
#define TIME_ON				247
#define TIME_OFF			246
#define FIRST_ON			245
#define FIRST_OFF			244
#define SECOND_ON			243
#define SECOND_OFF			242
#define BATCH_DONE			241
#define SHIFTED				240
#define MOVIE_START			239
#define MOVIE_STOP			238
#define MOVIE_CHANGE		237
#define FLOW_RAMP_START		236
#define FLOW_RAMP_STOP		235
#define FLOW_RAMP_CHANGE	234
#define OBJECT_LOCATION		233
#define BUFFER_SWAP			232

/*
 * parameter defines
 */
#define RA_CLKWISE			16
#define RA_CCLKWISE			32
#define RA_ONCE				0
#define RA_OSCIL			1
#define RA_LOOP				2
#define EACH_OBJECT			0
#define WHOLE_SCREEN		1
#define OBJ_OFF				0
#define OBJ_ON				1
#define ON_AFTER_RAMP		1
#define OFF_AFTER_RAMP		0
