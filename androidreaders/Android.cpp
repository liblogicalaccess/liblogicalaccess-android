#include "Android.hpp"
#include "logicalaccess/settings.hpp"
#include <string>
#include <jni.h>

namespace logicalaccess
{
    void Android::Init(JNIEnv *env)
    {
		//get library shared android dir to set correct plugin folder
		jclass cls = env->FindClass("com/islog/liblogicalaccess/NDKHelper");
		jclass jNDKhelper = (jclass) env->NewGlobalRef(cls);
		jmethodID getnativelibdir = env->GetStaticMethodID(jNDKhelper, "getNativeLibraryDirectory", "()Ljava/lang/String;");
		jstring jNativeDirResult = (jstring) env->CallStaticObjectMethod(jNDKhelper, getnativelibdir);
		env->DeleteLocalRef(cls);
		const char *s = env->GetStringUTFChars(jNativeDirResult, NULL);
		std::string nativeDir = s;
		env->ReleaseStringUTFChars(jNativeDirResult, s);

		logicalaccess::Settings::getInstance()->PluginFolders.clear();
		logicalaccess::Settings::getInstance()->PluginFolders.push_back(nativeDir);
    }
}
