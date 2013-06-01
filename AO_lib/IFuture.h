#pragma once

typedef enum FutureStatus_e
{
	FUTURE_READY,		/**< object is filled with data -> results can be read */
	FUTURE_NOT_READY	/**< object is not filled -> do not read data */
} FutureStatus_t;

/**
* Interface for future object.
*/
class IFuture
{
public:
	IFuture() {};
	virtual ~IFuture() {};
};