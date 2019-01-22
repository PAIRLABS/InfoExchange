import tensorflow as tf
import numpy as np
import time

if __name__ == "__main__":

    size_array = [8,16,32,64,128,256,512,1024,2048,4096,8192]
    loop_time = 10000
    for size in size_array:
        with tf.device("/cpu:0"):
            c = tf.Variable(tf.convert_to_tensor(np.eye(size),dtype=tf.float64),name='c-cpu')
            a = tf.constant(np.random.normal(loc=0.0,scale=0.01,size=(size,size)),name='a-cpu')
              

            i = tf.constant(1,name='i-cpu')
            n = tf.constant(loop_time,name='n-cpu')

            def cond(i,n,c,a):
                return i<n

            def loop(i,n,c,a):
                i=i+1
                c=c*a
                return i,n,c,a

            cpu_master = tf.while_loop(cond,loop,[i,n,c,a],name='loop-cpu')

        with tf.device("/gpu:0"):
            C = tf.Variable(tf.convert_to_tensor(np.eye(size),dtype=tf.float64),name='C-GPU')
            A = tf.constant(np.random.normal(loc=0.0,scale=0.01,size=(size,size)),name='A-GPU')


            I = tf.constant(1,name='I-GPU')
            N = tf.constant(loop_time,name='N-GPU')

            def COND(I,N,C,A):
                return I<N

            def LOOP(I,N,C,A):
                I=I+1
                C=C*A
                return I,N,C,A

            GPU_MASTER = tf.while_loop(COND,LOOP,[I,N,C,A],name='LOOP-GPU')

        init = tf.global_variables_initializer()

        with tf.Session() as sess:
            sess.run(init)

            t1=time.time()
            sess.run(cpu_master)


            t2=time.time()
            sess.run(GPU_MASTER)

            t3=time.time()
            print ("matrix size: %f" %size)
            print ("  CPU : %f s" %(t2-t1))
            print ("  GPU : %f s" %(t3-t2))
            print ("CPU : GPU = %f" %( (t2-t1)/(t3-t2) ) )
            print ("---------------")
