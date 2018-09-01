#include "Batch.h"

#include "RenderContext.h"

namespace DE {

Batch::Batch() {
	// TODO Auto-generated constructor stub

}

Batch::~Batch() {
	// TODO Auto-generated destructor stub
}

} /* namespace DE */

void DE::Batch::bind() {
	mVAO = RenderContext::createVAO();
	mVBOPosition = RenderContext::createVBO(nullptr, 3, 0);
	mVBOElemIndices = RenderContext::createEBO(nullptr);
	//mVBONormal = RenderContext::createVBO();
}
