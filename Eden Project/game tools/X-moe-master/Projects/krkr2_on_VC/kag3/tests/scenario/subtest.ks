@wait time=200
@macro name=cl
@call *
@endmacro
*test1|�Z�[�u1
@startanchor
@ct
�T�u���[�`���̃e�X�g[l]
@disablestore
disablestore���Ă݂�[l]
@cl target=*test2
�T�u���[�`������߂��� subtest_test1[l]
0 -> 5[copybookmark from=0 to=5][l]
erase 5[erasebookmark place=5][s]


*test2|�Z�[�u2
@ct
�T�u���[�`���̃e�X�g[l]
@load place=0 ask=true
@gotostart ask=true
@store enabled=false
store enabled=false[l]
@store enabled=true
store enabled=true[l]
@save place=4 ask=true
@cl target=*test3 storage=subtest2.ks
�T�u���[�`������߂��� subtest_test2[l]
@return

