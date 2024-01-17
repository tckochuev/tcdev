#ifndef IEXPORTER_AS_IMAGES
#define IEXPORTER_AS_IMAGES

#include <stdexcept>

//Some way of polymorphic error reporting can be used here.
//For example std::error_code (complex), pure int + virtual getErrorMessage() (certanly bad).
//Most modern way without throwing exception is monadic error reporting.

//Some implementation of error monad, for example: tc::err::Result.
template<typename V, typename E = std::exception_ptr>
struct ErrorMonad { //exposition only.
	static ErrorMonad makeError(E e);
	static ErrorMonad makeValue(V);

	bool isError() const;
	const V& v() const;
	const E& e() const;
};

class IExporterAsImages
{
public:
	using Path = std::string; //some path type.

	virtual ErrorMonad<void> exportAsImages(Path inputFile, Path outputDir) = 0;
	virtual ErrorMonad<void> waitExportAsImages() = 0;
};

class Stopped : public std::runtime_error {};

// not every implementation can be stopped.
class IStoppableExporterAsImages : public IExporterAsImages
{
public:
	virtual ErrorMonad<void> stopExportAsImages() = 0;
};

class IPreviewExporter
{
public:
	using Path = std::string; //some path type.

	virtual ErrorMonad<void> exportPreview(Path inputFile, Path outputDir) = 0;
	virtual ErrorMonad<void> waitPreviewExport() = 0;
};

// not every implementation can be stopped.
class IStoppablePreviewExporter : public IPreviewExporter
{
public:
	virtual ErrorMonad<void> stopPreviewExport() = 0;
};

class SomeDocumentExporter : public IStoppableExporterAsImages, public IStoppablePreviewExporter
{
public:
	class RuntimeError1 : public std::runtime_error {
	public:
		RuntimeError1() : std::runtime_error("Some runtime error") {}
	};
	class LogicError1 : public std::logic_error {
	public:
		LogicError1() : std::logic_error("Some logic error") {}
	};
	/*...*/
	/* class ErrorN : ... */

	ErrorMonad<void> exportAsImages(IExporterAsImages::Path inputFile, IExporterAsImages::Path outputDir)
	{
		auto checkPrecond = []{return true;};
		if(!checkPrecond()) {
			return ErrorMonad<void>::makeError(std::make_exception_ptr(LogicError1()));
		}
		auto doExport = []{return false;};
		if(!doExport()) {
			return ErrorMonad<void>::makeError(std::make_exception_ptr(RuntimeError1()));
		}
		else {
			return ErrorMonad<void>::makeValue();
		}
	}

	/*override other things*/
};

/// We DECOUPLE way of calling members of interface from interface itself, USER of interface decides, if member should be called in separated thread.
/// -> So task creation SHOULD not be HARDCODED inside interface.

class SomeUser
{
public:
	struct Task
	{
		template<typename C>
		void start(C callable);
	};

	ErrorMonad<void> startExportAsImages(IExporterAsImages& exporter)
	{
		IExporterAsImages::Path inputFile = "path obtained from somewhere";
		IExporterAsImages::Path outputDir = "path obtained from somewhere";
		auto onError = [](std::exception_ptr e) {/*error parsing*/};
		auto onSuccess = []{/*...*/};
		m_task.start([&] {
			auto res = exporter.exportAsImages(inputFile, outputDir);
			if(!res.isError())
			{
				onSuccess();
			}
			else
			{
				onError(res.e());
			}
		});
	}

	ErrorMonad<void> stopExportAsImages(IStoppableExporterAsImages& exporter)
	{
		return exporter.stopExportAsImages();
	};

	Task m_task;
};

#endif
