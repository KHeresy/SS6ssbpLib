#include <windows.h>
#include <stdio.h>
#include <iostream>

#include <GL/glew.h>
#include <GL/glut.h>

#include "./SSPlayer/SS6Player.h"

//��ʃT�C�Y
#define WIDTH (1280)
#define HEIGHT (720)

//FPS����p
int nowtime = 0;	//�o�ߎ���
int drawtime = 0;	//�O��̎���

// SS�v���C���[
ss::Player *ssplayer;
ss::ResourceManager *resman;

//glut�̃R�[���o�b�N�֐�
void mouse(int button, int state, int x, int y);
void keyboard(unsigned char key, int x, int y);
void idle(void);
void disp(void);

//�A�v���P�[�V�����̐���
void Init();
void update(float dt);
void relese(void);
void draw(void);


//�A�v���P�[�V�����̃��C���֐��֐�
int main(int argc, char ** argv) 
{
	//���C�u����������
	glutInit(&argc, argv);				//GLUT�̏�����
	//�E�B���h�E�쐬
	glutInitWindowPosition(100, 50);	//�E�B���h�E�ʒu�ݒ�
	glutInitWindowSize(WIDTH, HEIGHT);	//�E�B���h�E�T�C�Y�ݒ�
	glutInitDisplayMode(GLUT_RGBA | GLUT_STENCIL | GLUT_DEPTH | GLUT_DOUBLE);	//�g�p����o�b�t�@��ݒ�
	glutCreateWindow("Sprite Studio SS6ssbpLib Sample");		//�E�B���h�E�^�C�g��

	GLenum err;
	err = glewInit();					//GLEW�̏�����
	if (err != GLEW_OK) {
		std::cerr << glewGetErrorString(err) << '\n';
		return 0;
	}

	glClearColor(0.0, 0.0, 0.2, 1.0);		//�w�i�F

	//���荞�ݐݒ�	
	glutIdleFunc(idle);			//�A�C�h���R�[���o�b�N�ݒ�
	glutDisplayFunc(disp);		//�\���R�[���o�b�N�ݒ�
	glutKeyboardFunc(keyboard);	//�L�[�{�[�h���̓R�[���o�b�N�ݒ�
	glutMouseFunc(mouse);		//�}�E�X���̓R�[���o�b�N�ݒ�

	Init();

	glutMainLoop();				//���C�����[�v
	return 0;
}

//�L�[�{�[�h���̓R�[���o�b�N
void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 27:	//esc
		relese();					//�A�v���P�[�V�����I��
		exit(0);
		break;
	default:
		break;
	}
}

//�}�E�X���̓R�[���o�b�N
void mouse(int button, int state, int x, int y)
{
}

//�A�C�h���R�[���o�b�N
void idle(void) 
{
	//FPS�̐ݒ�
	nowtime = glutGet(GLUT_ELAPSED_TIME);//�o�ߎ��Ԃ��擾
	int wait = nowtime - drawtime;
	if (wait >= 16)
	{
		update((float)wait / 1000.0f);
		glutPostRedisplay();
		drawtime = nowtime;
	}
}

//�`��R�[���o�b�N
void disp(void)
{
	//�����_�����O�J�n���̏�����
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);	//�t���[���o�b�t�@�̃N���A
	glDisable(GL_STENCIL_TEST);							//�X�e���V�������ɂ���
	glEnable(GL_DEPTH_TEST);							//�[�x�o�b�t�@��L���ɂ���
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);	//�t���[���o�b�t�@�֊e�F�̏������݂�ݒ�

	draw();

	//�I������
	glDisable(GL_DEPTH_TEST);	//�[�x�e�X�g�𖳌��ɂ���
	glDisable(GL_ALPHA_TEST);	//�A���t�@�e�X�g�����ɂ���
	glDisable(GL_TEXTURE_2D);	//�e�N�X�`������
	glDisable(GL_BLEND);		//�u�����h�𖳌��ɂ���

	glutSwapBuffers();
}

//�A�v���P�[�V��������������
void Init()
{
	/**********************************************************************************

	SpriteStudio�A�j���[�V�����\���̃T���v���R�[�h
	Visual Studio Community 2017�œ�����m�F���Ă��܂��B
	WindowsSDK(�f�X�N�g�b�vC++ x86�����x64�p��Windows10 SDK)���C���X�g�[������K�v������܂�
	�v���W�F�N�g��NuGet��glut��������nupengl.core��ǉ����Ă��������B

	ssbp��png������΍Đ����鎖���ł��܂����AResources�t�H���_��sspj���܂܂�Ă��܂��B

	**********************************************************************************/

	//�v���C���[���g�p����O�̏���������
	//���̏����̓A�v���P�[�V�����̏������łP�x�����s���Ă��������B
	ss::SSPlatformInit();
	//Y�����̐ݒ�ƃE�B���h�E�T�C�Y�ݒ���s���܂�
	ss::SSSetPlusDirection(ss::PLUS_UP, WIDTH, HEIGHT);
	//���\�[�X�}�l�[�W���̍쐬
	resman = ss::ResourceManager::getInstance();
	//�v���C���[���g�p����O�̏��������������܂�


	//�v���C���[�̍쐬
	ssplayer = ss::Player::create();

	//�A�j���f�[�^�����\�[�X�ɒǉ�

	//���ꂼ��̃v���b�g�t�H�[���ɍ��킹���p�X�֕ύX���Ă��������B
	resman->addData("Resources/character_template_comipo/character_template1.ssbp");
	//�v���C���[�Ƀ��\�[�X�����蓖��
	ssplayer->setData("character_template1");        // ssbp�t�@�C�����i�g���q�s�v�j
	//�Đ����郂�[�V������ݒ�
	ssplayer->play("character_template_3head/stance");				 // �A�j���[�V���������w��(ssae��/�A�j���[�V���������\�A�ڂ����͌�q)


	//�\���ʒu��ݒ�
	ssplayer->setPosition(WIDTH / 2, HEIGHT / 2);
	ssplayer->setScale(0.5f, 0.5f);
}

//�A�v���P�[�V�����X�V
void update(float dt)
{
	//�v���C���[�̍X�V�A�����͑O��̍X�V��������o�߂�������
	ssplayer->update(dt);
}

//�A�v���P�[�V�����`��
void draw(void)
{
	//�v���C���[�̕`��
	ssplayer->draw();
}

//�A�v���P�[�V�����I������
void relese(void)
{
	//SSPlayer�̍폜
	delete (ssplayer);
	delete (resman);
	ss::SSPlatformRelese( );
}

