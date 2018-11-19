@Library("islog-helper") _

pipeline {
    agent none

    options {
        gitLabConnection('Gitlab Pontos')
        disableConcurrentBuilds()
    }

    environment {
        LINUX_DOCKER_IMAGE_NAME = 'docker-registry.islog.com:5000/conan-recipes-support:cis-latest'
        ANDROID_DOCKER_IMAGE_NAME = 'docker-registry.islog.com:5000/conan-recipes-support-android:cis-latest'
        PACKAGE_NAME = "LogicalAccessAndroid/2.1.0@cis/${BRANCH_NAME}"
    }

    /**
     * This Jenkinsfile driven build will generate Conan package for multiple
     * combination of settings and options.
     */

    stages {
        stage('Android ARM') {
            // Build with no additional option, simply using a Conan profile file.
            // Also for now we assume any profile is android, which wont always be true
            steps {
                script {
                    node('linux') {
                        docker.image(ANDROID_DOCKER_IMAGE_NAME).inside {
                            checkout scm
                            dir('androidreaders') {
                                lla.conanPerformAllWithProfile('../../arm_clang_android_21')
                            }
                        }
                    }
                }
            }
        }
        stage('Android x86') {
            // Build with no additional option, simply using a Conan profile file.
            // Also for now we assume any profile is android, which wont always be true
            steps {
                script {
                    node('linux') {
                        docker.image(ANDROID_DOCKER_IMAGE_NAME).inside {
                            checkout scm
                            dir('androidreaders') {
                                lla.conanPerformAllWithProfile('../../x86_clang_android_21')
                            }
                        }
                    }
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
