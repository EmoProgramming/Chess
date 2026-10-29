synch:
	em++ web/cpp/chess.cpp -o web/js/chess.js     -s MODULARIZE=1     -s EXPORT_ES6=1     -s 'EXPORTED_FUNCTIONS=["_getPieceJS","_allocateMemory","_freeMemory","_generateLegalMovesJS"]'     -s 'EXPORTED_RUNTIME_METHODS=["HEAP32"]'
	rsync -av web/ granskye@u1-1.ms.mff.cuni.cz:/afs/ms/u/g/granskye/WWW/chess

