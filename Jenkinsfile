@Library("islog-helper") _

pipeline {
    agent none

    options {
        gitLabConnection('Gitlab Pontos')
        disableConcurrentBuilds()
    }

    environment {
        ANDROID_DOCKER_IMAGE_NAME = 'docker-registry.islog.com:5000/conan-recipes-support-android:cis-latest'
        PACKAGE_NAME = "LogicalAccessAndroid/2.1.0@islog/${BRANCH_NAME}"
    }

    stages {
        stage('Android') {
            steps {
                script {
                    lla.startJobForProfiles(['lla/arm_clang_android_21_debug',
                                             'lla/arm_clang_android_21',
                                             'lla/x86_clang_android_21'])
                }
            }
        }
    }

    post {
        changed {
            script {
                if (currentBuild.currentResult == 'FAILURE') {
                    // Other values: SUCCESS, UNSTABLE
                    // Send an email only if the build status has changed from green/unstable to red
                    emailext subject: '$DEFAULT_SUBJECT',
                            body: '$DEFAULT_CONTENT',
                            recipientProviders: [
                                    [$class: 'CulpritsRecipientProvider'],
                                    [$class: 'DevelopersRecipientProvider'],
                                    [$class: 'RequesterRecipientProvider']
                            ],
                            replyTo: 'cis@islog.com',
                            to: 'reports@islog.com'
                }
            }
        }
    }
}
