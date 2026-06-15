package com.example.lib.lichess.model

/**
 * Cloud evaluation result from the Lichess cloud-eval API.
 *
 * @property fen The FEN string of the evaluated position.
 * @property depth The depth of the analysis in plies.
 * @property knodes The number of nodes evaluated (in thousands).
 * @property pvs The principal variations with their evaluations.
 */
data class CloudEvalResult(
	val fen: String,
	val depth: Int,
	val knodes: Int,
	val pvs: List<Pv>,
)

/**
 * A principal variation with its evaluation score.
 *
 * Either [cp] (centipawns) or [mate] (moves to mate) will be present,
 * depending on whether the position has a forced checkmate.
 *
 * @property moves The sequence of moves in UCI notation (space-separated).
 * @property cp The evaluation in centipawns (null if checkmate).
 * @property mate Moves to forced checkmate (null if not a checkmate).
 *   Positive values mean white delivers mate, negative values mean black does.
 */
data class Pv(
	val moves: String,
	val cp: Int?,
	val mate: Int?,
)
