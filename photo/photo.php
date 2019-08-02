<?php
// 获取对象存储临时秘钥，计算签名
    function api_user_photo_sts(){
        // 配置参数
        $config = array(
            'url' => 'https://sts.tencentcloudapi.com/',
            'domain' => 'sts.tencentcloudapi.com',
            'proxy' => '',
            'secretId' => '', // 固定密钥
            'secretKey' => '', // 固定密钥
            'bucket' => '', // 换成你的 bucket
            'region' => 'ap-guangzhou', // 换成 bucket 所在园区
            'durationSeconds' => 1800, // 密钥有效期
            'allowPrefix' => '*', // 这里改成允许的路径前缀，可以根据自己网站的用户登录态判断允许上传的目录，例子：* 或者 a/* 或者 a.jpg
            // 密钥的权限列表。简单上传和分片需要以下的权限，其他权限列表请看 https://cloud.tencent.com/document/product/436/31923
            'allowActions' => array (
                // 所有 action 请看文档 https://cloud.tencent.com/document/product/436/31923
                // 简单上传
                'name/cos:PutObject',
                'name/cos:PostObject',
                // 分片上传
                'name/cos:InitiateMultipartUpload',
                'name/cos:ListMultipartUploads',
                'name/cos:ListParts',
                'name/cos:UploadPart',
                'name/cos:CompleteMultipartUpload'
            )
        );
        
        // 获取临时密钥，计算签名
        $sts = new \Home\Model\QcloudCos; // 在 Model 创建了 COS 相关的类
        $tempKeys = $sts->getTempKeys($config);
        $this->ajaxReturn($tempKeys) ; 
    }