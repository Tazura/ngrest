#include <ngrest/utils/Exception.h>
#include <ngrest/utils/Log.h>
#include <ngrest/common/Message.h>

#include "Deployment.h"
#include "Transport.h"
#include "Engine.h"

namespace ngrest {

// replace callback to write response with appropriate transport
class EngineHookCallback: public MessageCallback
{
public:
    EngineHookCallback(MessageContext* context_):
        context(context_), origCallback(context_->callback)
    {
        context->callback = this;
    }

    void success(MessageContext* context)
    {
        context->transport->writeResponse(context->pool, context->request, context->response);
        context->callback = origCallback;
        context->callback->success(context);
    }

    void error(const Exception& error)
    {
        context->callback = origCallback;
        context->callback->error(error);
    }

    MessageContext* const context;
    MessageCallback* const origCallback;
};


Engine::Engine(Deployment& deployment_):
    deployment(deployment_)
{
}

void Engine::dispatchMessage(MessageContext* context)
{
    NGREST_ASSERT_PARAM(context);
    NGREST_ASSERT_NULL(context->request);
    NGREST_ASSERT_NULL(context->response);
    NGREST_ASSERT_NULL(context->callback);

    try {
        // this will replace context callback and restore it after dispatching the message
        context->pool.alloc<EngineHookCallback>(context);

        if (context->request->body) {
            context->request->node = context->transport->parseRequest(context->pool, context->request);
            NGREST_ASSERT(context->request->node, "Failed to read request"); // should never throw
        }

        deployment.dispatchMessage(context);
    } catch (const Exception& err) {
        LogWarning() << /*err.getFileLine() << " " << */err.getFunction() << " : " << err.what();
        context->callback->error(err);
    }
}

} // namespace ngrest
