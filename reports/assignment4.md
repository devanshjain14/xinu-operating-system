#### Which future mode was used for fibonachchi and why?

FUTURE_SHARED mode was used for fibonachchi because FUTURE_SHARED can be polled in multiple threads and in FUTURE_SHARED if any clone of this Shared has completed execution, returns its result immediately without blocking.
And FUTURE_SHARED gives us the leverage to use mutable resources after they have been produced in the past, which is not the case in FUTURE_EXCLUSIVE.
In FUTURE_EXCLUSIVE, once a value is consumed, it can not be brought back, while in FUTURE_SHARED, it is possible to get the value produced and then can be put into the array.
